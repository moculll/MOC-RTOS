#include <DebugTrace/DebugTrace.h>
#include <osImpl/DebugTraceImpl.h>
#include <drivers/uart/pl011.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>
namespace MOC {


void DebugTraceImpl::uartOutput(const char *data)
{
    print_uart0(data);

}


} /* MOC */