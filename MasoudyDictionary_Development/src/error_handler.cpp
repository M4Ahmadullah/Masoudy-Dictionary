#include "error_handler.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <functional>

namespace MasoudyError {

// Initialize singleton instance
std::unique_ptr<ErrorHandler> ErrorHandler::instance = nullptr;

ErrorHandler::ErrorHandler() {
    // Initialize error handler
}

ErrorHandler::~ErrorHandler() {
    // Cleanup
}

ErrorHandler& ErrorHandler::getInstance() {
    if (!instance) {
        instance = std::make_unique<ErrorHandler>();
    }
    return *instance;
}

void ErrorHandler::reportError(ErrorSeverity severity, ErrorCategory category, 
                              const std::string& message, const std::string& details,
                              const std::string& function, int line) {
    ErrorInfo error(severity, category, message);
    error.details = details;
    error.function = function;
    error.lineNumber = line;
    error.timestamp = getCurrentTimestamp();
    
    // Log the error
    logError(error);
    
    // Attempt recovery if callback is set
    if (recoveryCallback) {
        recoveryCallback(error);
    }
}

void ErrorHandler::setLogCallback(std::function<void(const ErrorInfo&)> callback) {
    logCallback = callback;
}

void ErrorHandler::logError(const ErrorInfo& error) {
    // Default logging to console
    std::cout << formatError(error) << std::endl;
    
    // Call custom callback if set
    if (logCallback) {
        logCallback(error);
    }
}

void ErrorHandler::setRecoveryCallback(std::function<bool(const ErrorInfo&)> callback) {
    recoveryCallback = callback;
}

bool ErrorHandler::attemptRecovery(const ErrorInfo& error) {
    if (recoveryCallback) {
        return recoveryCallback(error);
    }
    return false;
}

std::string ErrorHandler::formatError(const ErrorInfo& error) {
    std::stringstream ss;
    
    ss << "[" << getSeverityString(error.severity) << "] ";
    ss << "[" << getCategoryString(error.category) << "] ";
    ss << error.timestamp << " ";
    
    if (!error.function.empty()) {
        ss << "(" << error.function;
        if (error.lineNumber > 0) {
            ss << ":" << error.lineNumber;
        }
        ss << ") ";
    }
    
    ss << error.message;
    
    if (!error.details.empty()) {
        ss << " - " << error.details;
    }
    
    return ss.str();
}

bool ErrorHandler::isCriticalError(const ErrorInfo& error) {
    return error.severity == ErrorSeverity::CRITICAL;
}

void ErrorHandler::clearErrors() {
    // Clear any stored error state
}

std::string ErrorHandler::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string ErrorHandler::getSeverityString(ErrorSeverity severity) {
    switch (severity) {
        case ErrorSeverity::INFO: return "INFO";
        case ErrorSeverity::WARNING: return "WARNING";
        case ErrorSeverity::ERROR: return "ERROR";
        case ErrorSeverity::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

std::string ErrorHandler::getCategoryString(ErrorCategory category) {
    switch (category) {
        case ErrorCategory::AUTHENTICATION: return "AUTH";
        case ErrorCategory::NETWORK: return "NET";
        case ErrorCategory::STEALTH: return "STEALTH";
        case ErrorCategory::SCREEN_CAPTURE: return "SCREEN";
        case ErrorCategory::MEMORY: return "MEMORY";
        case ErrorCategory::SYSTEM: return "SYSTEM";
        case ErrorCategory::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

} // namespace MasoudyError 