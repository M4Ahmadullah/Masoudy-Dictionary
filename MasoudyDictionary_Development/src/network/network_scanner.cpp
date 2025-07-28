#include "network_scanner.h"
#include <iostream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>
#endif

namespace MasoudyCore {

NetworkScanner::NetworkScanner() 
    : m_scanning(false)
    , m_paused(false)
    , m_timeout(500)
    , m_startIP(1)
    , m_endIP(254) {
    
#ifdef _WIN32
    // Initialize Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

NetworkScanner::~NetworkScanner() {
    StopScan();
#ifdef _WIN32
    WSACleanup();
#endif
}

bool NetworkScanner::Initialize() {
    try {
        // Get local network information
        if (!GetLocalNetworkInfo()) {
            return false;
        }
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Network scanner initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void NetworkScanner::Shutdown() {
    StopScan();
}

bool NetworkScanner::StartScan(const std::string& baseIP, int startIP, int endIP, int timeout) {
    if (m_scanning) {
        return false;
    }
    
    m_baseIP = baseIP;
    m_startIP = startIP;
    m_endIP = endIP;
    m_timeout = timeout;
    m_scanning = true;
    m_paused = false;
    m_discoveredHosts.clear();
    
    // Start scanning thread
    m_scanThread = std::thread(&NetworkScanner::ScanThreadProc, this);
    
    return true;
}

bool NetworkScanner::StopScan() {
    if (!m_scanning) {
        return true;
    }
    
    m_scanning = false;
    m_paused = false;
    
    if (m_scanThread.joinable()) {
        m_scanThread.join();
    }
    
    return true;
}

bool NetworkScanner::PauseScan() {
    if (!m_scanning) {
        return false;
    }
    
    m_paused = true;
    return true;
}

bool NetworkScanner::ResumeScan() {
    if (!m_scanning) {
        return false;
    }
    
    m_paused = false;
    return true;
}

bool NetworkScanner::IsScanning() const {
    return m_scanning;
}

bool NetworkScanner::IsPaused() const {
    return m_paused;
}

std::vector<NetworkHost> NetworkScanner::GetDiscoveredHosts() const {
    return m_discoveredHosts;
}

void NetworkScanner::ScanThreadProc() {
    for (int ip = m_startIP; ip <= m_endIP && m_scanning; ++ip) {
        if (m_paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        
        std::string targetIP = m_baseIP + "." + std::to_string(ip);
        
        if (PingHost(targetIP)) {
            NetworkHost host;
            host.ip = targetIP;
            host.hostname = GetHostname(targetIP);
            host.os = DetectOS(targetIP);
            host.services = ScanServices(targetIP);
            host.accessible = true;
            
            m_discoveredHosts.push_back(host);
        }
        
        // Small delay to avoid overwhelming the network
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

bool NetworkScanner::PingHost(const std::string& ip) {
    try {
        // Create socket
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            return false;
        }
        
        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = m_timeout / 1000;
        timeout.tv_usec = (m_timeout % 1000) * 1000;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
        
        // Set up address
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(80); // Try HTTP port
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        
        // Try to connect
        int result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
        
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        
        return result == 0;
    }
    catch (const std::exception& e) {
        return false;
    }
}

std::string NetworkScanner::GetHostname(const std::string& ip) {
    try {
        struct hostent* he = gethostbyaddr(ip.c_str(), ip.length(), AF_INET);
        if (he) {
            return std::string(he->h_name);
        }
    }
    catch (const std::exception& e) {
        // Ignore errors
    }
    
    return "";
}

std::string NetworkScanner::DetectOS(const std::string& ip) {
    // Simplified OS detection based on common ports
    std::vector<int> ports = {22, 23, 25, 80, 443, 3389, 8080};
    
    for (int port : ports) {
        if (CheckPort(ip, port)) {
            switch (port) {
                case 22: return "Linux/Unix (SSH)";
                case 23: return "Telnet Server";
                case 25: return "Mail Server";
                case 80: return "Web Server";
                case 443: return "Secure Web Server";
                case 3389: return "Windows (RDP)";
                case 8080: return "Web Server (Alt)";
                default: return "Unknown";
            }
        }
    }
    
    return "Unknown";
}

std::vector<std::string> NetworkScanner::ScanServices(const std::string& ip) {
    std::vector<std::string> services;
    std::vector<std::pair<int, std::string>> commonPorts = {
        {21, "FTP"},
        {22, "SSH"},
        {23, "Telnet"},
        {25, "SMTP"},
        {53, "DNS"},
        {80, "HTTP"},
        {110, "POP3"},
        {143, "IMAP"},
        {443, "HTTPS"},
        {993, "IMAPS"},
        {995, "POP3S"},
        {1433, "MSSQL"},
        {3306, "MySQL"},
        {3389, "RDP"},
        {5432, "PostgreSQL"},
        {8080, "HTTP-Alt"},
        {8443, "HTTPS-Alt"}
    };
    
    for (const auto& port : commonPorts) {
        if (CheckPort(ip, port.first)) {
            services.push_back(port.second);
        }
    }
    
    return services;
}

bool NetworkScanner::CheckPort(const std::string& ip, int port) {
    try {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            return false;
        }
        
        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
        
        // Set up address
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        
        // Try to connect
        int result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
        
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        
        return result == 0;
    }
    catch (const std::exception& e) {
        return false;
    }
}

bool NetworkScanner::GetLocalNetworkInfo() {
    try {
#ifdef _WIN32
        // Get local IP using Windows API
        char hostname[256];
        gethostname(hostname, sizeof(hostname));
        
        struct addrinfo hints, *result;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        
        if (getaddrinfo(hostname, nullptr, &hints, &result) == 0) {
            struct sockaddr_in* sockaddr_ipv4 = (struct sockaddr_in*)result->ai_addr;
            m_localIP = inet_ntoa(sockaddr_ipv4->sin_addr);
            
            // Extract base IP (first 3 octets)
            size_t lastDot = m_localIP.find_last_of('.');
            if (lastDot != std::string::npos) {
                m_baseIP = m_localIP.substr(0, lastDot);
            }
            
            freeaddrinfo(result);
            return true;
        }
#else
        // Get local IP using getifaddrs
        struct ifaddrs* ifaddr;
        if (getifaddrs(&ifaddr) == -1) {
            return false;
        }
        
        for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == nullptr) continue;
            
            if (ifa->ifa_addr->sa_family == AF_INET) {
                struct sockaddr_in* addr = (struct sockaddr_in*)ifa->ifa_addr;
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);
                
                // Skip loopback
                if (strcmp(ip, "127.0.0.1") != 0) {
                    m_localIP = ip;
                    
                    // Extract base IP (first 3 octets)
                    size_t lastDot = m_localIP.find_last_of('.');
                    if (lastDot != std::string::npos) {
                        m_baseIP = m_localIP.substr(0, lastDot);
                    }
                    
                    freeifaddrs(ifaddr);
                    return true;
                }
            }
        }
        
        freeifaddrs(ifaddr);
#endif
        
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to get local network info: " << e.what() << std::endl;
        return false;
    }
}

std::string NetworkScanner::GetLocalIP() const {
    return m_localIP;
}

std::string NetworkScanner::GetBaseIP() const {
    return m_baseIP;
}

// Internal methods implementation
bool NetworkScanner::InitializeScan() {
    return Initialize();
}

bool NetworkScanner::CleanupScan() {
    StopScan();
    return true;
}

bool NetworkScanner::ScanHost(const std::string& ip) {
    return PingHost(ip);
}

bool NetworkScanner::ScanPort(const std::string& ip, int port) {
    return CheckPort(ip, port);
}

bool NetworkScanner::ValidateIP(const std::string& ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) == 1;
}

std::string NetworkScanner::ReverseDNSLookup(const std::string& ip) {
    return GetHostname(ip);
}

std::string NetworkScanner::DNSLookup(const std::string& hostname) {
    try {
        struct hostent* he = gethostbyname(hostname.c_str());
        if (he) {
            struct in_addr addr;
            addr.s_addr = *(unsigned long*)he->h_addr_list[0];
            return inet_ntoa(addr);
        }
    }
    catch (const std::exception& e) {
        // Ignore errors
    }
    
    return "";
}

bool NetworkScanner::ScanLocalNetwork(std::vector<NetworkHost>& hosts) {
    // Legacy method for compatibility
    if (!Initialize()) {
        return false;
    }
    
    // Start scan with default settings
    if (!StartScan(m_baseIP, 1, 254, 500)) {
        return false;
    }
    
    // Wait for scan to complete
    while (IsScanning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Get results
    hosts = GetDiscoveredHosts();
    return true;
}

bool NetworkScanner::PerformOSFingerprinting(const std::string& target) {
    try {
        // Create socket for OS fingerprinting
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            return false;
        }
        
        // Set socket options for fingerprinting
        int opt = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
        
        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
        
        // Connect to target
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(80); // HTTP port for fingerprinting
        
        if (inet_pton(AF_INET, target.c_str(), &serverAddr.sin_addr) <= 0) {
            close(sock);
            return false;
        }
        
        if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            close(sock);
            return false;
        }
        
        // Analyze TCP/IP stack characteristics
        // TTL values, window sizes, TCP options, etc.
        // This is a simplified implementation
        
        close(sock);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "OS fingerprinting failed: " << e.what() << std::endl;
        return false;
    }
}

bool NetworkScanner::EnumerateServices(const std::string& target) {
    try {
        // Common ports to scan
        std::vector<int> commonPorts = {21, 22, 23, 25, 53, 80, 110, 143, 443, 993, 995, 3306, 3389, 5432, 8080};
        
        for (int port : commonPorts) {
            if (IsServiceRunning(target, port)) {
                // Add service to discovered services
                std::string serviceName = GetServiceName(port);
                // Store in discovered services list
            }
        }
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Service enumeration failed: " << e.what() << std::endl;
        return false;
    }
}

bool NetworkScanner::PerformVulnerabilityAssessment(const std::string& target) {
    try {
        // Basic vulnerability assessment
        // Check for common vulnerabilities
        
        // 1. Check for open ports
        std::vector<int> criticalPorts = {22, 23, 3389, 3306, 5432};
        for (int port : criticalPorts) {
            if (IsServiceRunning(target, port)) {
                // Log potential vulnerability
                std::cout << "Potential vulnerability: Port " << port << " is open on " << target << std::endl;
            }
        }
        
        // 2. Check for default services
        if (IsServiceRunning(target, 22)) {
            // SSH service detected
            std::cout << "SSH service detected on " << target << std::endl;
        }
        
        if (IsServiceRunning(target, 3389)) {
            // RDP service detected
            std::cout << "RDP service detected on " << target << std::endl;
        }
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Vulnerability assessment failed: " << e.what() << std::endl;
        return false;
    }
}

bool NetworkScanner::PerformPassiveDiscovery() {
    try {
        // Listen for network broadcasts
        // ARP requests, DHCP broadcasts, etc.
        
        // This is a simplified implementation
        // In a real implementation, you would:
        // 1. Listen for ARP broadcasts
        // 2. Monitor DHCP traffic
        // 3. Listen for network discovery protocols
        
        std::cout << "Performing passive network discovery..." << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Passive discovery failed: " << e.what() << std::endl;
        return false;
    }
}

bool NetworkScanner::PerformActiveScanning() {
    try {
        // Perform active network scanning
        // Ping sweep, port scanning, etc.
        
        std::cout << "Performing active network scanning..." << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Active scanning failed: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> NetworkScanner::GetDetectedServices(const std::string& target) {
    std::vector<std::string> services;
    
    // Check common services
    if (IsServiceRunning(target, 22)) services.push_back("SSH");
    if (IsServiceRunning(target, 23)) services.push_back("Telnet");
    if (IsServiceRunning(target, 80)) services.push_back("HTTP");
    if (IsServiceRunning(target, 443)) services.push_back("HTTPS");
    if (IsServiceRunning(target, 3389)) services.push_back("RDP");
    if (IsServiceRunning(target, 3306)) services.push_back("MySQL");
    if (IsServiceRunning(target, 5432)) services.push_back("PostgreSQL");
    if (IsServiceRunning(target, 8080)) services.push_back("HTTP-Proxy");
    
    return services;
}

std::string NetworkScanner::GetOSInfo(const std::string& target) {
    // Simplified OS detection
    // In a real implementation, this would analyze TCP/IP stack characteristics
    
    // Check for Windows-specific services
    if (IsServiceRunning(target, 3389)) {
        return "Windows (RDP detected)";
    }
    
    // Check for Linux-specific services
    if (IsServiceRunning(target, 22)) {
        return "Linux/Unix (SSH detected)";
    }
    
    return "Unknown OS";
}

bool NetworkScanner::IsServiceRunning(const std::string& target, int port) {
    try {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            return false;
        }
        
        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
        
        // Connect to target port
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        
        if (inet_pton(AF_INET, target.c_str(), &serverAddr.sin_addr) <= 0) {
            close(sock);
            return false;
        }
        
        if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            close(sock);
            return false;
        }
        
        close(sock);
        return true;
    }
    catch (const std::exception& e) {
        return false;
    }
}

bool NetworkScanner::AnalyzeNetworkTopology() {
    try {
        std::cout << "Analyzing network topology..." << std::endl;
        // Implement network topology analysis
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Network topology analysis failed: " << e.what() << std::endl;
        return false;
    }
}

bool NetworkScanner::DetectNetworkDevices() {
    try {
        std::cout << "Detecting network devices..." << std::endl;
        // Implement network device detection
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Network device detection failed: " << e.what() << std::endl;
        return false;
    }
}

bool NetworkScanner::IdentifyNetworkSegments() {
    try {
        std::cout << "Identifying network segments..." << std::endl;
        // Implement network segment identification
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Network segment identification failed: " << e.what() << std::endl;
        return false;
    }
}

std::string NetworkScanner::GetServiceName(int port) {
    switch (port) {
        case 21: return "FTP";
        case 22: return "SSH";
        case 23: return "Telnet";
        case 25: return "SMTP";
        case 53: return "DNS";
        case 80: return "HTTP";
        case 110: return "POP3";
        case 143: return "IMAP";
        case 443: return "HTTPS";
        case 993: return "IMAPS";
        case 995: return "POP3S";
        case 3306: return "MySQL";
        case 3389: return "RDP";
        case 5432: return "PostgreSQL";
        case 8080: return "HTTP-Proxy";
        default: return "Unknown";
    }
}

} // namespace MasoudyCore 