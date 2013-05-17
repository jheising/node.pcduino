#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "gpio-wrapper.h"
#include "gpio.h"

using namespace v8;

static GPIO gpio;

void GPIOInit(Handle<Object> exports)
{
	Local<FunctionTemplate> tpl = FunctionTemplate::New();

	tpl->Set("INPUT", Number::New(INPUT));
	tpl->Set("OUTPUT", Number::New(OUTPUT));
	tpl->Set("INPUT_PU", Number::New(INPUT_PU));
	tpl->Set("HIGH", Number::New(HIGH));
	tpl->Set("LOW", Number::New(LOW));

	tpl->Set(String::NewSymbol("pinMode"),
          FunctionTemplate::New(pinMode)->GetFunction());
	tpl->Set(String::NewSymbol("digitalWrite"),
          FunctionTemplate::New(digitalWrite)->GetFunction());
    tpl->Set(String::NewSymbol("digitalRead"),
              FunctionTemplate::New(digitalRead)->GetFunction());

    Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
    exports->Set(String::NewSymbol("gpio"), constructor);
}

Handle<Value> pinMode(const Arguments& args)
{
	HandleScope scope;

	if (args.Length() < 2) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
      }

    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
      }

    gpio.pinMode(args[0]->NumberValue(), (PIN_MODE)args[1]->NumberValue());

    return scope.Close(Undefined());
}

Handle<Value> digitalWrite(const Arguments& args)
{
	HandleScope scope;

	if (args.Length() < 2) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
      }

    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
      }

    gpio.digitalWrite(args[0]->NumberValue(), (PIN_STATE)args[1]->NumberValue());

    return scope.Close(Undefined());
}

Handle<Value> digitalRead(const Arguments& args)
{
	HandleScope scope;

    if (args.Length() < 1) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
      }

    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
      }

    int state = gpio.digitalRead(args[0]->NumberValue());

    return scope.Close(Number::New(state));
}

/*GPIOWrapper::GPIOWrapper() {};
GPIOWrapper::~GPIOWrapper() {};

void GPIOWrapper::Init(Handle<Object> exports) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("gpio"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("pinMode"),
      FunctionTemplate::New(pinMode)->GetFunction());

  tpl->PrototypeTemplate()->Set(String::NewSymbol("digitalRead"),
      FunctionTemplate::New(digitalRead)->GetFunction());

  tpl->PrototypeTemplate()->Set(String::NewSymbol("digitalWrite"),
      FunctionTemplate::New(digitalWrite)->GetFunction());

  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("gpio"), constructor);
}

Handle<Value> GPIOWrapper::New(const Arguments& args) {
  HandleScope scope;

  GPIOWrapper* obj = new GPIOWrapper();
  obj->gpio = GPIO();
  obj->Wrap(args.This());

  return args.This();
}

v8::Handle<v8::Value> GPIOWrapper::pinMode(const v8::Arguments& args)
{
	HandleScope scope;

	if (args.Length() < 2) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
      }

    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
      }

    GPIOWrapper* obj = ObjectWrap::Unwrap<GPIOWrapper>(args.This());
    obj->gpio.pinMode(args[0]->NumberValue(), (PIN_MODE)args[1]->NumberValue());

    return scope.Close(Undefined());
}

v8::Handle<v8::Value> GPIOWrapper::digitalWrite(const v8::Arguments& args)
{
	HandleScope scope;

	if (args.Length() < 2) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
      }

    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
      }

    GPIOWrapper* obj = ObjectWrap::Unwrap<GPIOWrapper>(args.This());
    obj->gpio.digitalWrite(args[0]->NumberValue(), (PIN_STATE)args[1]->NumberValue());

    return scope.Close(Undefined());
}

v8::Handle<v8::Value> GPIOWrapper::digitalRead(const v8::Arguments& args)
{
	HandleScope scope;

    if (args.Length() < 1) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
      }

    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
      }

    GPIOWrapper* obj = ObjectWrap::Unwrap<GPIOWrapper>(args.This());
    int state = obj->gpio.digitalRead(args[0]->NumberValue());

    return scope.Close(Number::New(state));
}*/