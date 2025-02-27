#pragma once
#include <DebugTrace/DebugTrace.h>

namespace MOC {

class DebugTraceImpl final : public DebugTrace {
public:
    
    DebugTraceImpl() {}
    ~DebugTraceImpl() {}
    
    static void init()
    {
        DebugTrace::setInstance(std::make_unique<DebugTraceImpl>());
    }
private:
    
    void uartOutput(const char *data) override;

};

}