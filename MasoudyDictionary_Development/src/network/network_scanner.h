#pragma once

#include <string>
#include <vector>
#include <thread>
#include "masoudy_core.h" // Include to use NetworkHost from there

namespace MasoudyCore {

class NetworkScanner {
public:
    NetworkScanner();
    ~NetworkScanner();
    
    // Initialization
    bool Initialize();
    void Shutdown();
    
    // Scanning control
    bool StartScan(const std::string& baseIP, int startIP, int endIP, int timeout);
    bool StopScan();
    bool PauseScan();
    bool ResumeScan();
    
    // Status
    bool IsScanning() const;
    bool IsPaused() const;
    
    // Results
    std::vector<NetworkHost> GetDiscoveredHosts() const;
    
    // Legacy method for compatibility
    bool ScanLocalNetwork(std::vector<NetworkHost>& hosts);
    
    // Network info
    std::string GetLocalIP() const;
    std::string GetBaseIP() const;

    // Advanced scanning methods
    bool PerformOSFingerprinting(const std::string& target);
    bool EnumerateServices(const std::string& target);
    bool PerformVulnerabilityAssessment(const std::string& target);
    bool PerformPassiveDiscovery();
    bool PerformActiveScanning();
    
    // Service detection
    std::vector<std::string> GetDetectedServices(const std::string& target);
    std::string GetOSInfo(const std::string& target);
    bool IsServiceRunning(const std::string& target, int port);
    
    // Advanced network analysis
    bool AnalyzeNetworkTopology();
    bool DetectNetworkDevices();
    bool IdentifyNetworkSegments();
    
    // Utility methods
    std::string GetServiceName(int port);

private:
    // Scanning methods
    void ScanThreadProc();
    bool PingHost(const std::string& ip);
    std::string GetHostname(const std::string& ip);
    std::string DetectOS(const std::string& ip);
    std::vector<std::string> ScanServices(const std::string& ip);
    bool CheckPort(const std::string& ip, int port);
    
    // Network info
    bool GetLocalNetworkInfo();
    
    // Internal methods
    bool InitializeScan();
    bool CleanupScan();
    bool ScanHost(const std::string& ip);
    bool ScanPort(const std::string& ip, int port);
    bool ValidateIP(const std::string& ip);
    std::string ReverseDNSLookup(const std::string& ip);
    std::string DNSLookup(const std::string& hostname);
    
    // Member variables
    bool m_scanning;
    bool m_paused;
    int m_timeout;
    int m_startIP;
    int m_endIP;
    std::string m_baseIP;
    std::string m_localIP;
    std::vector<NetworkHost> m_discoveredHosts;
    std::thread m_scanThread;
};

} // namespace MasoudyCore 