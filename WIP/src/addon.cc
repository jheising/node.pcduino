#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "onewire/onewire-wrapper.h"
#include "gpio/gpio-wrapper.h"

using namespace v8;

void InitAll(Handle<Object> exports)
{
  OneWireWrapper::Init(exports);
  GPIOInit(exports);
}
NODE_MODULE(pcduino, InitAll)