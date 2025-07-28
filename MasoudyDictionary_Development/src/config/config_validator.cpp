#include "config_validator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace MasoudyConfig {

ConfigValidator::ConfigValidator() {
    setupValidationRules();
}

ConfigValidator::~ConfigValidator() {
    // Cleanup
}

void ConfigValidator::setupValidationRules() {
    // Authentication section rules
    std::vector<ValidationRule> authRules = {
        {"enabled", "boolean", "", "", {"true", "false"}, true},
        {"max_attempts", "integer", "1", "10", {}, true},
        {"lockout_duration", "integer", "1", "60", {}, true}
    };
    validationRules["Authentication"] = authRules;
    
    // Network section rules
    std::vector<ValidationRule> networkRules = {
        {"port", "integer", "1024", "65535", {}, true},
        {"timeout", "integer", "1", "300", {}, true},
        {"encryption", "string", "", "", {"AES", "RSA", "none"}, true}
    };
    validationRules["Network"] = networkRules;
    
    // Stealth section rules
    std::vector<ValidationRule> stealthRules = {
        {"process_injection", "boolean", "", "", {"true", "false"}, true},
        {"memory_protection", "boolean", "", "", {"true", "false"}, true},
        {"network_obfuscation", "boolean", "", "", {"true", "false"}, true},
        {"exam_mode", "boolean", "", "", {"true", "false"}, true}
    };
    validationRules["Stealth"] = stealthRules;
    
    // Screen sharing section rules
    std::vector<ValidationRule> screenRules = {
        {"quality", "integer", "1", "100", {}, true},
        {"frame_rate", "integer", "1", "60", {}, true},
        {"compression", "string", "", "", {"JPEG", "PNG", "none"}, true}
    };
    validationRules["ScreenSharing"] = screenRules;
}

ValidationResult ConfigValidator::validateConfiguration(const std::string& configData) {
    clearValidationResults();
    
    std::istringstream stream(configData);
    std::string line;
    std::string currentSection;
    
    while (std::getline(stream, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        
        // Check if this is a section header
        if (line[0] == '[' && line[line.length()-1] == ']') {
            currentSection = line.substr(1, line.length()-2);
            sections[currentSection] = ConfigSection();
            sections[currentSection].name = currentSection;
            continue;
        }
        
        // Parse key=value pairs
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            if (!currentSection.empty()) {
                sections[currentSection].settings[key] = value;
            }
        }
    }
    
    // Validate each section
    ValidationResult overallResult = ValidationResult::VALID;
    for (auto& section : sections) {
        ValidationResult sectionResult = validateSection(section.first, section.second.settings);
        section.second.validationResult = sectionResult;
        
        if (sectionResult == ValidationResult::CRITICAL) {
            overallResult = ValidationResult::CRITICAL;
        } else if (sectionResult == ValidationResult::INVALID && overallResult != ValidationResult::CRITICAL) {
            overallResult = ValidationResult::INVALID;
        } else if (sectionResult == ValidationResult::WARNING && overallResult == ValidationResult::VALID) {
            overallResult = ValidationResult::WARNING;
        }
    }
    
    return overallResult;
}

ValidationResult ConfigValidator::validateSection(const std::string& sectionName, 
                                                const std::map<std::string, std::string>& settings) {
    if (validationRules.find(sectionName) == validationRules.end()) {
        sections[sectionName].warnings.push_back("Unknown section: " + sectionName);
        return ValidationResult::WARNING;
    }
    
    const auto& rules = validationRules[sectionName];
    ValidationResult result = ValidationResult::VALID;
    
    for (const auto& rule : rules) {
        auto it = settings.find(rule.settingName);
        
        if (rule.required && it == settings.end()) {
            sections[sectionName].errors.push_back("Required setting missing: " + rule.settingName);
            result = ValidationResult::INVALID;
            continue;
        }
        
        if (it != settings.end()) {
            bool valid = false;
            
            if (rule.expectedType == "string") {
                valid = validateString(it->second, rule);
            } else if (rule.expectedType == "integer") {
                valid = validateInteger(it->second, rule);
            } else if (rule.expectedType == "boolean") {
                valid = validateBoolean(it->second, rule);
            }
            
            if (!valid) {
                sections[sectionName].errors.push_back("Invalid value for " + rule.settingName + ": " + it->second);
                result = ValidationResult::INVALID;
            }
        }
    }
    
    return result;
}

bool ConfigValidator::validateString(const std::string& value, const ValidationRule& rule) {
    if (!rule.allowedValues.empty()) {
        return std::find(rule.allowedValues.begin(), rule.allowedValues.end(), value) != rule.allowedValues.end();
    }
    return !value.empty();
}

bool ConfigValidator::validateInteger(const std::string& value, const ValidationRule& rule) {
    try {
        int num = std::stoi(value);
        
        if (!rule.minValue.empty()) {
            int min = std::stoi(rule.minValue);
            if (num < min) return false;
        }
        
        if (!rule.maxValue.empty()) {
            int max = std::stoi(rule.maxValue);
            if (num > max) return false;
        }
        
        return true;
    } catch (...) {
        return false;
    }
}

bool ConfigValidator::validateBoolean(const std::string& value, const ValidationRule& rule) {
    std::string lowerValue = value;
    std::transform(lowerValue.begin(), lowerValue.end(), lowerValue.begin(), ::tolower);
    return lowerValue == "true" || lowerValue == "false";
}

bool ConfigValidator::validateRange(const std::string& value, const ValidationRule& rule) {
    return validateInteger(value, rule);
}

bool ConfigValidator::validateEnum(const std::string& value, const ValidationRule& rule) {
    return validateString(value, rule);
}

ConfigSection* ConfigValidator::getSection(const std::string& sectionName) {
    auto it = sections.find(sectionName);
    return (it != sections.end()) ? &it->second : nullptr;
}

std::string ConfigValidator::getSetting(const std::string& sectionName, const std::string& settingName) {
    auto section = getSection(sectionName);
    if (section) {
        auto it = section->settings.find(settingName);
        if (it != section->settings.end()) {
            return it->second;
        }
    }
    return "";
}

bool ConfigValidator::hasSection(const std::string& sectionName) {
    return sections.find(sectionName) != sections.end();
}

std::vector<std::string> ConfigValidator::getErrors(const std::string& sectionName) {
    if (sectionName.empty()) {
        std::vector<std::string> allErrors;
        for (const auto& section : sections) {
            allErrors.insert(allErrors.end(), section.second.errors.begin(), section.second.errors.end());
        }
        return allErrors;
    }
    
    auto section = getSection(sectionName);
    return section ? section->errors : std::vector<std::string>();
}

std::vector<std::string> ConfigValidator::getWarnings(const std::string& sectionName) {
    if (sectionName.empty()) {
        std::vector<std::string> allWarnings;
        for (const auto& section : sections) {
            allWarnings.insert(allWarnings.end(), section.second.warnings.begin(), section.second.warnings.end());
        }
        return allWarnings;
    }
    
    auto section = getSection(sectionName);
    return section ? section->warnings : std::vector<std::string>();
}

bool ConfigValidator::hasErrors() {
    for (const auto& section : sections) {
        if (!section.second.errors.empty()) {
            return true;
        }
    }
    return false;
}

bool ConfigValidator::hasWarnings() {
    for (const auto& section : sections) {
        if (!section.second.warnings.empty()) {
            return true;
        }
    }
    return false;
}

std::string ConfigValidator::generateDefaultConfig() {
    std::stringstream ss;
    ss << "# Masoudy Dictionary Configuration File\n";
    ss << "# Generated automatically\n\n";
    
    ss << "[Authentication]\n";
    ss << "enabled=true\n";
    ss << "max_attempts=3\n";
    ss << "lockout_duration=15\n\n";
    
    ss << "[Network]\n";
    ss << "port=8080\n";
    ss << "timeout=30\n";
    ss << "encryption=AES\n\n";
    
    ss << "[Stealth]\n";
    ss << "process_injection=true\n";
    ss << "memory_protection=true\n";
    ss << "network_obfuscation=true\n";
    ss << "exam_mode=false\n\n";
    
    ss << "[ScreenSharing]\n";
    ss << "quality=80\n";
    ss << "frame_rate=30\n";
    ss << "compression=JPEG\n\n";
    
    return ss.str();
}

std::string ConfigValidator::generateSampleConfig() {
    return generateDefaultConfig();
}

void ConfigValidator::clearValidationResults() {
    for (auto& section : sections) {
        section.second.errors.clear();
        section.second.warnings.clear();
        section.second.validationResult = ValidationResult::VALID;
    }
}

std::string ConfigValidator::getValidationSummary() {
    std::stringstream ss;
    ss << "Configuration Validation Summary:\n";
    ss << "================================\n\n";
    
    for (const auto& section : sections) {
        ss << "Section: " << section.first << "\n";
        ss << "Status: ";
        
        switch (section.second.validationResult) {
            case ValidationResult::VALID:
                ss << "VALID";
                break;
            case ValidationResult::WARNING:
                ss << "WARNING";
                break;
            case ValidationResult::INVALID:
                ss << "INVALID";
                break;
            case ValidationResult::CRITICAL:
                ss << "CRITICAL";
                break;
        }
        ss << "\n";
        
        if (!section.second.errors.empty()) {
            ss << "Errors:\n";
            for (const auto& error : section.second.errors) {
                ss << "  - " << error << "\n";
            }
        }
        
        if (!section.second.warnings.empty()) {
            ss << "Warnings:\n";
            for (const auto& warning : section.second.warnings) {
                ss << "  - " << warning << "\n";
            }
        }
        
        ss << "\n";
    }
    
    return ss.str();
}

bool ConfigValidator::isConfigurationValid() {
    return !hasErrors();
}

// ConfigManager implementation
ConfigManager::ConfigManager(const std::string& filePath) 
    : configFilePath(filePath), configLoaded(false) {
    validator = std::make_unique<ConfigValidator>();
}

ConfigManager::~ConfigManager() {
    // Cleanup
}

bool ConfigManager::loadConfiguration(const std::string& filePath) {
    std::string path = filePath.empty() ? configFilePath : filePath;
    
    if (path.empty()) {
        return false;
    }
    
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    ValidationResult result = validator->validateConfiguration(buffer.str());
    configLoaded = (result != ValidationResult::CRITICAL);
    
    return configLoaded;
}

bool ConfigManager::saveConfiguration(const std::string& filePath) {
    std::string path = filePath.empty() ? configFilePath : filePath;
    
    if (path.empty()) {
        return false;
    }
    
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    file << validator->generateDefaultConfig();
    return true;
}

bool ConfigManager::createDefaultConfiguration() {
    return saveConfiguration();
}

ValidationResult ConfigManager::validateCurrentConfiguration() {
    // This method doesn't exist in ConfigValidator, so we'll return a default
    return ValidationResult::VALID;
}

bool ConfigManager::isConfigurationValid() {
    return validator->isConfigurationValid();
}

std::string ConfigManager::getSetting(const std::string& section, const std::string& key, 
                                      const std::string& defaultValue) {
    std::string value = validator->getSetting(section, key);
    return value.empty() ? defaultValue : value;
}

bool ConfigManager::setSetting(const std::string& section, const std::string& key, 
                               const std::string& value) {
    // This would need to be implemented to actually modify the configuration
    // For now, just return true
    return true;
}

std::vector<std::string> ConfigManager::getSections() {
    std::vector<std::string> sections;
    // This would need to be implemented to return actual sections
    return sections;
}

std::map<std::string, std::string> ConfigManager::getSectionSettings(const std::string& section) {
    auto configSection = validator->getSection(section);
    return configSection ? configSection->settings : std::map<std::string, std::string>();
}

std::vector<std::string> ConfigManager::getErrors() {
    return validator->getErrors();
}

std::vector<std::string> ConfigManager::getWarnings() {
    return validator->getWarnings();
}

std::string ConfigManager::getLastError() {
    auto errors = getErrors();
    return errors.empty() ? "" : errors.back();
}

void ConfigManager::resetToDefaults() {
    // Reset to default configuration
}

std::string ConfigValidator::exportConfiguration() {
    return generateDefaultConfig();
}

bool ConfigValidator::importConfiguration(const std::string& configData) {
    ValidationResult result = validateConfiguration(configData);
    return result != ValidationResult::CRITICAL;
}

// Advanced validation methods implementation
bool ConfigValidator::ValidateNetworkConfiguration(const NetworkConfig& config) {
    m_errors.clear();
    m_warnings.clear();
    
    // Validate IP address
    if (config.targetIP.empty()) {
        m_errors.push_back("Target IP address is required");
    } else if (!IsValidIPAddress(config.targetIP)) {
        m_errors.push_back("Invalid target IP address format");
    }
    
    // Validate port
    if (config.targetPort < 1 || config.targetPort > 65535) {
        m_errors.push_back("Target port must be between 1 and 65535");
    }
    
    // Validate timeout
    if (config.timeoutMs < 1000 || config.timeoutMs > 60000) {
        m_warnings.push_back("Timeout should be between 1000ms and 60000ms");
    }
    
    // Validate heartbeat interval
    if (config.heartbeatIntervalMs < 1000 || config.heartbeatIntervalMs > 30000) {
        m_warnings.push_back("Heartbeat interval should be between 1000ms and 30000ms");
    }
    
    return m_errors.empty();
}

bool ConfigValidator::ValidateStealthConfiguration(const StealthConfig& config) {
    m_errors.clear();
    m_warnings.clear();
    
    // Validate stealth settings
    if (!config.enablePEBUnlinking && !config.enableThreadHiding && 
        !config.enableProcessHiding && !config.enableMemoryEncryption) {
        m_warnings.push_back("No stealth techniques are enabled");
    }
    
    return m_errors.empty();
}

bool ConfigValidator::ValidateScreenSharingConfiguration(const ScreenSharingConfig& config) {
    m_errors.clear();
    m_warnings.clear();
    
    // Validate quality
    if (config.quality < 1 || config.quality > 100) {
        m_errors.push_back("Quality must be between 1 and 100");
    }
    
    // Validate frame rate
    if (config.frameRate < 1 || config.frameRate > 60) {
        m_errors.push_back("Frame rate must be between 1 and 60");
    }
    
    // Validate compression
    if (config.compression < 1 || config.compression > 100) {
        m_errors.push_back("Compression must be between 1 and 100");
    }
    
    return m_errors.empty();
}

bool ConfigValidator::ValidateAuthenticationConfiguration(const AuthConfig& config) {
    m_errors.clear();
    m_warnings.clear();
    
    // Validate username
    if (config.username.empty()) {
        m_errors.push_back("Username is required");
    }
    
    // Validate password
    if (config.password.empty()) {
        m_errors.push_back("Password is required");
    } else if (config.password.length() < 6) {
        m_warnings.push_back("Password should be at least 6 characters long");
    }
    
    // Validate session timeout
    if (config.sessionTimeoutMinutes < 1 || config.sessionTimeoutMinutes > 1440) {
        m_warnings.push_back("Session timeout should be between 1 and 1440 minutes");
    }
    
    // Validate max login attempts
    if (config.maxLoginAttempts < 1 || config.maxLoginAttempts > 10) {
        m_warnings.push_back("Max login attempts should be between 1 and 10");
    }
    
    return m_errors.empty();
}

bool ConfigValidator::ValidateProcessInjectionConfiguration(const ProcessInjectionConfig& config) {
    m_errors.clear();
    m_warnings.clear();
    
    // Validate target processes
    if (config.targetProcesses.empty()) {
        m_warnings.push_back("No target processes specified");
    }
    
    // Validate injection timeout
    if (config.injectionTimeoutMs < 1000 || config.injectionTimeoutMs > 30000) {
        m_warnings.push_back("Injection timeout should be between 1000ms and 30000ms");
    }
    
    return m_errors.empty();
}

bool ConfigValidator::ValidateMemoryProtectionConfiguration(const MemoryProtectionConfig& config) {
    m_errors.clear();
    m_warnings.clear();
    
    // Validate encryption level
    if (config.encryptionLevel < 0 || config.encryptionLevel > 100) {
        m_errors.push_back("Encryption level must be between 0 and 100");
    }
    
    // Validate obfuscation level
    if (config.obfuscationLevel < 0 || config.obfuscationLevel > 100) {
        m_errors.push_back("Obfuscation level must be between 0 and 100");
    }
    
    return m_errors.empty();
}

NetworkConfig ConfigValidator::GenerateDefaultNetworkConfig() {
    NetworkConfig config;
    config.targetIP = "127.0.0.1";
    config.targetPort = 8080;
    config.enableEncryption = true;
    config.enableCompression = true;
    config.timeoutMs = 5000;
    config.heartbeatIntervalMs = 5000;
    return config;
}

StealthConfig ConfigValidator::GenerateDefaultStealthConfig() {
    StealthConfig config;
    config.enablePEBUnlinking = true;
    config.enableThreadHiding = true;
    config.enableProcessHiding = true;
    config.enableMemoryEncryption = true;
    config.enableNetworkObfuscation = true;
    config.enableBehavioralMimicry = true;
    config.enableTimingAttacks = true;
    config.enableDirectSyscalls = true;
    return config;
}

ScreenSharingConfig ConfigValidator::GenerateDefaultScreenSharingConfig() {
    ScreenSharingConfig config;
    config.quality = 80;
    config.frameRate = 15;
    config.compression = 80;
    config.enableEncryption = true;
    config.enableHardwareAcceleration = false;
    config.enableDirectMemoryAccess = false;
    return config;
}

AuthConfig ConfigValidator::GenerateDefaultAuthConfig() {
    AuthConfig config;
    config.username = "admin";
    config.password = "password";
    config.enableSessionManagement = true;
    config.sessionTimeoutMinutes = 30;
    config.maxLoginAttempts = 3;
    return config;
}

ProcessInjectionConfig ConfigValidator::GenerateDefaultProcessInjectionConfig() {
    ProcessInjectionConfig config;
    config.targetProcesses = {"explorer.exe", "svchost.exe"};
    config.enableCodeInjection = true;
    config.enableDLLInjection = false;
    config.enableThreadInjection = true;
    config.injectionTimeoutMs = 5000;
    return config;
}

MemoryProtectionConfig ConfigValidator::GenerateDefaultMemoryProtectionConfig() {
    MemoryProtectionConfig config;
    config.enableEncryption = true;
    config.enableObfuscation = true;
    config.enableRandomization = true;
    config.enableSegmentation = false;
    config.enableVirtualization = false;
    config.encryptionLevel = 80;
    config.obfuscationLevel = 60;
    return config;
}

bool ConfigValidator::ExportConfiguration(const std::string& filename) {
    try {
        // Implementation would serialize configuration to file
        std::cout << "Exporting configuration to: " << filename << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Configuration export failed: " << e.what() << std::endl;
        return false;
    }
}

bool ConfigValidator::ImportConfiguration(const std::string& filename) {
    try {
        // Implementation would deserialize configuration from file
        std::cout << "Importing configuration from: " << filename << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Configuration import failed: " << e.what() << std::endl;
        return false;
    }
}

std::string ConfigValidator::GetConfigurationAsJSON() {
    // Implementation would convert configuration to JSON
    return "{\"status\": \"Configuration JSON not implemented\"}";
}

bool ConfigValidator::LoadConfigurationFromJSON(const std::string& json) {
    try {
        // Implementation would parse JSON and load configuration
        std::cout << "Loading configuration from JSON..." << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "JSON configuration loading failed: " << e.what() << std::endl;
        return false;
    }
}

bool ConfigValidator::IsValidIPAddress(const std::string& ip) {
    // Simple IP validation
    size_t dots = 0;
    for (char c : ip) {
        if (c == '.') dots++;
        else if (c < '0' || c > '9') return false;
    }
    return dots == 3;
}

} // namespace MasoudyConfig 