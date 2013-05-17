#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "onewire-wrapper.h"

using namespace v8;

OneWireWrapper::OneWireWrapper(int pinNumber) : p_onewire(new OneWire((uint8_t)pinNumber)) {}
OneWireWrapper::~OneWireWrapper() {delete p_onewire;}

void OneWireWrapper::Init(Handle<Object> exports) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("onewire"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("reset"),
      FunctionTemplate::New(reset)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("skip"),
      FunctionTemplate::New(skip)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("search"),
      FunctionTemplate::New(search)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("resetSearch"),
        FunctionTemplate::New(resetSearch)->GetFunction());

  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("onewire"), constructor);
}

Handle<Value> OneWireWrapper::New(const Arguments& args) {
  HandleScope scope;

	if (args.Length() < 1) {
            ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
            return scope.Close(Undefined());
          }

        if (!args[0]->IsNumber()) {
            ThrowException(Exception::TypeError(String::New("Wrong arguments")));
            return scope.Close(Undefined());
          }

  OneWireWrapper* obj = new OneWireWrapper(args[0]->NumberValue());
  obj->Wrap(args.This());

  return args.This();
}

v8::Handle<v8::Value> OneWireWrapper::reset(const v8::Arguments& args)
{
	HandleScope scope;

    OneWireWrapper* obj = ObjectWrap::Unwrap<OneWireWrapper>(args.This());

    int r = obj->p_onewire->reset();

    return scope.Close(Number::New(r));
}

v8::Handle<v8::Value> OneWireWrapper::skip(const v8::Arguments& args)
{
	HandleScope scope;

    OneWireWrapper* obj = ObjectWrap::Unwrap<OneWireWrapper>(args.This());

    obj->p_onewire->skip();

    return scope.Close(Undefined());
}

v8::Handle<v8::Value> OneWireWrapper::search(const v8::Arguments& args)
{
	HandleScope scope;

    OneWireWrapper* obj = ObjectWrap::Unwrap<OneWireWrapper>(args.This());

    unsigned char addr[8];
    //addr[8] = '\0';

    if(!obj->p_onewire->search(addr))
	{
		obj->p_onewire->reset_search();
		return scope.Close(Undefined());
	}

	if ( obj->p_onewire->crc8( addr, 7) != addr[7]) {
       // CRC is not valid!
       ThrowException(Exception::TypeError(String::New("onewire CRC not valid.")));
       return scope.Close(Undefined());
    }

    if ( addr[0] != 0x10 && addr[0] != 0x28) {
        // Device is not recognized
        ThrowException(Exception::TypeError(String::New("onewire device not recognized.")));
        return scope.Close(Undefined());
    }

    //char hexAddr[20];
    //sprintf(hexAddr, "%02x", addr);
    //return scope.Close(String::New(hexAddr));

    unsigned long long longAddr;
    memcpy(&longAddr, addr, sizeof(addr));
    return scope.Close(Number::New(longAddr));
}

v8::Handle<v8::Value> OneWireWrapper::resetSearch(const v8::Arguments& args)
{
	HandleScope scope;

    OneWireWrapper* obj = ObjectWrap::Unwrap<OneWireWrapper>(args.This());

    obj->p_onewire->reset_search();

    return scope.Close(Undefined());
}