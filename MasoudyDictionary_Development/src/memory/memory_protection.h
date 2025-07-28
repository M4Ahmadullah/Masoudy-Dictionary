#pragma once

#include <cstddef>
#include <vector>
#include <string>

namespace MasoudyCore {

enum class ProtectionType {
    ENCRYPTION,
    OBFUSCATION,
    READ_ONLY,
    EXECUTE_ONLY,
    GUARD_PAGE,
    RANDOMIZATION,
    SEGMENTATION,
    VIRTUALIZATION
};

struct MemoryRegion {
    void* address;
    size_t size;
    ProtectionType type;
    bool active;
    ProtectionType protection;  // For compatibility with implementation
    bool isProtected;           // For compatibility with implementation
};

struct ProtectionSettings {
    int encryptionLevel;
    int obfuscationLevel;
    bool enableRandomization;
    bool enableSegmentation;
    bool enableVirtualization;
    
    ProtectionSettings() : encryptionLevel(1), obfuscationLevel(1), 
                          enableRandomization(false), enableSegmentation(false), 
                          enableVirtualization(false) {}
};

class MemoryProtection {
public:
    MemoryProtection();
    ~MemoryProtection();
    
    bool Initialize();
    void Shutdown();
    
    bool ProtectMemoryRegion(void* address, size_t size, ProtectionType type);
    bool UnprotectMemoryRegion(void* address);
    bool EncryptMemoryRegion(void* address, size_t size);
    bool DecryptMemoryRegion(void* address, size_t size);
    bool ObfuscateMemoryRegion(void* address, size_t size);
    bool DeobfuscateMemoryRegion(void* address, size_t size);

    // Settings
    bool SetProtectionSettings(const ProtectionSettings& settings);
    ProtectionSettings GetProtectionSettings() const;
    void SetEncryptionLevel(int level);
    void SetObfuscationLevel(int level);
    int GetEncryptionLevel() const;
    int GetObfuscationLevel() const;

    // Status and information
    bool IsInitialized() const;
    bool IsRegionProtected(void* address) const;
    std::string GetLastError() const;
    std::vector<MemoryRegion> GetProtectedRegions() const;

    // Management
    bool ClearAllProtections();
    bool ValidateProtections();
    
private:
    bool m_initialized;
    std::vector<MemoryRegion> m_protectedRegions;
    ProtectionSettings m_settings;
    std::string m_lastError;
    
    // Internal methods
    bool InitializeProtectionSystem();
    void ShutdownProtectionSystem();
    bool ApplyEncryption(void* address, size_t size);
    bool ApplyObfuscation(void* address, size_t size);
    bool ApplyRandomization(void* address, size_t size);
    bool ApplySegmentation(void* address, size_t size);
    bool ApplyVirtualization(void* address, size_t size);
    bool RemoveProtection(void* address);
    MemoryRegion* FindRegion(void* address) const;
    void AddRegion(const MemoryRegion& region);
    void RemoveRegion(void* address);
    bool ValidateAddress(void* address, size_t size);
};

// Utility functions
std::string GetProtectionTypeName(ProtectionType type);
bool IsProtectionSupported(ProtectionType type);
size_t GetOptimalRegionSize();
bool IsMemoryAccessible(void* address, size_t size);
void* AllocateProtectedMemory(size_t size);
void FreeProtectedMemory(void* address);

} // namespace MasoudyCore 