#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <string>
#include <exception>
#include <functional>
#include <memory>

namespace MasoudyError {

// Error severity levels
enum class ErrorSeverity {
    INFO,       // Informational messages
    WARNING,    // Warning messages
    ERROR,      // Error messages
    CRITICAL    // Critical errors
};

// Error categories
enum class ErrorCategory {
    AUTHENTICATION,    // Authentication errors
    NETWORK,          // Network communication errors
    STEALTH,          // Stealth operation errors
    SCREEN_CAPTURE,   // Screen capture errors
    MEMORY,           // Memory management errors
    SYSTEM,           // System-level errors
    UNKNOWN           // Unknown errors
};

// Error information structure
struct ErrorInfo {
    ErrorSeverity severity;
    ErrorCategory category;
    std::string message;
    std::string details;
    std::string timestamp;
    std::string function;
    int lineNumber;
    
    ErrorInfo(ErrorSeverity sev, ErrorCategory cat, const std::string& msg)
        : severity(sev), category(cat), message(msg) {}
};

// Error handler class
class ErrorHandler {
private:
    static std::unique_ptr<ErrorHandler> instance;
    
    // Error logging callback
    std::function<void(const ErrorInfo&)> logCallback;
    
    // Error recovery strategies
    std::function<bool(const ErrorInfo&)> recoveryCallback;
    
    // Helper methods
    std::string getCurrentTimestamp();
    std::string getSeverityString(ErrorSeverity severity);
    std::string getCategoryString(ErrorCategory category);

public:
    // Constructor (made public for singleton)
    ErrorHandler();
    
    // Singleton access
    static ErrorHandler& getInstance();
    
    // Error reporting
    void reportError(ErrorSeverity severity, ErrorCategory category, 
                    const std::string& message, const std::string& details = "",
                    const std::string& function = "", int line = 0);
    
    // Error logging
    void setLogCallback(std::function<void(const ErrorInfo&)> callback);
    void logError(const ErrorInfo& error);
    
    // Error recovery
    void setRecoveryCallback(std::function<bool(const ErrorInfo&)> callback);
    bool attemptRecovery(const ErrorInfo& error);
    
    // Utility methods
    std::string formatError(const ErrorInfo& error);
    bool isCriticalError(const ErrorInfo& error);
    void clearErrors();
    
    // Destructor
    ~ErrorHandler();
};

// Error exception class
class MasoudyException : public std::exception {
private:
    ErrorInfo errorInfo;
    
public:
    MasoudyException(ErrorSeverity severity, ErrorCategory category, 
                    const std::string& message, const std::string& details = "")
        : errorInfo(severity, category, message) {
        errorInfo.details = details;
    }
    
    const char* what() const noexcept override {
        return errorInfo.message.c_str();
    }
    
    const ErrorInfo& getErrorInfo() const {
        return errorInfo;
    }
};

// Error reporting macros
#define REPORT_ERROR(severity, category, message) \
    MasoudyError::ErrorHandler::getInstance().reportError( \
        MasoudyError::ErrorSeverity::severity, \
        MasoudyError::ErrorCategory::category, \
        message, "", __FUNCTION__, __LINE__)

#define REPORT_ERROR_WITH_DETAILS(severity, category, message, details) \
    MasoudyError::ErrorHandler::getInstance().reportError( \
        MasoudyError::ErrorSeverity::severity, \
        MasoudyError::ErrorCategory::category, \
        message, details, __FUNCTION__, __LINE__)

#define THROW_ERROR(severity, category, message) \
    throw MasoudyError::MasoudyException( \
        MasoudyError::ErrorSeverity::severity, \
        MasoudyError::ErrorCategory::category, \
        message)

#define THROW_ERROR_WITH_DETAILS(severity, category, message, details) \
    throw MasoudyError::MasoudyException( \
        MasoudyError::ErrorSeverity::severity, \
        MasoudyError::ErrorCategory::category, \
        message, details)

} // namespace MasoudyError

#endif // ERROR_HANDLER_H 