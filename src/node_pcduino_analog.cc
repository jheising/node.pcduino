#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "Arduino.h"

using namespace v8;

Handle<Value> analogWrite(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 2) {
      ThrowException(Exception::TypeError(v8::String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
      ThrowException(Exception::TypeError(v8::String::New("Wrong arguments")));
      return scope.Close(Undefined());
    }

    int pin = args[0]->NumberValue();
    int value = args[1]->NumberValue();

    analogWrite(pin, value);

  return scope.Close(Undefined());
}

Handle<Value> analogRead(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1) {
      ThrowException(Exception::TypeError(v8::String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    if (!args[0]->IsNumber()) {
      ThrowException(Exception::TypeError(v8::String::New("Wrong arguments")));
      return scope.Close(Undefined());
    }

    int pin = args[0]->NumberValue();

   int value = analogRead(pin);

   Local<Number> num = Number::New(value);
   return scope.Close(num);
}

void Init(Handle<Object> exports)
{
	init();

  exports->Set(v8::String::NewSymbol("analogWrite"),
      FunctionTemplate::New(analogWrite)->GetFunction());

	exports->Set(v8::String::NewSymbol("analogRead"),
            FunctionTemplate::New(analogRead)->GetFunction());
}

NODE_MODULE(node_pcduino_analog, Init)