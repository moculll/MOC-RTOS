#pragma once
#include <memory>
#include <string>
#include <map>
#include <cstdarg>
#include <cstring>
namespace MOC {

class DebugTrace {
public:

    template <typename ...Args>
    static inline int printf(const char* fmt, Args ...args) {
        return getInstance().debugPrint(fmt, args...);
    }

    template <typename ...Args>
    static inline int printfEx(int level, const char* fmt, Args ...args) {
        return getInstance().debugPrintEx(level, fmt, args...);
    }

    static int addLogLevel(std::string_view moduleNameView, int level) 
    {
        std::string moduleName(moduleNameView);
        getInstance().logLevelMap[level] = std::move(moduleName);
    }

    static inline void setInstance(std::unique_ptr<DebugTrace> newInstance)
    {
        instance = std::move(newInstance);
    }

    static inline DebugTrace &getInstance()
    {
        if (!instance) {
            instance = std::make_unique<DebugTrace>();
        }
        return *instance;
    }

    
    virtual ~DebugTrace() {}
    DebugTrace() {}
private:

    virtual int debugPrintEx(int level, const char* fmt, ...)
    {
        char buffer[100];
        int headerLength = 0;
        
        auto it = logLevelMap.find(level);
        if (it != logLevelMap.end()) {
            headerLength = std::sprintf(buffer, "[%d][%s] ", it->first, it->second.c_str());
        }
        else {
            headerLength = std::sprintf(buffer, "[-][-] ");
        }

        va_list args;
        va_start(args, fmt);
        int len = vsnprintf(buffer + headerLength, sizeof(buffer) - headerLength, fmt, args);
        va_end(args);

        strncat(buffer, "\r\n", len);
        uartOutput(buffer);
        return strlen(buffer);
    }

    virtual int debugPrint(const char* fmt, ...)
    {
        char buffer[100];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        uartOutput(buffer);
        return strlen(buffer);
    }

    virtual void uartOutput(const char *data) {}
    

    inline static std::unique_ptr<DebugTrace> instance;
    
    /**
     * int: logLevel
     * std::string: moduleName
     */
    std::map<int, std::string> logLevelMap;
    

};



} /* MOC */