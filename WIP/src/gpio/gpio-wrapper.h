#ifndef GPIO_WRAPPER_H
#define GPIO_WRAPPER_H

#include <node.h>
#include <v8.h>

using namespace v8;

void GPIOInit(Handle<Object> exports);
Handle<Value> pinMode(const Arguments& args);
Handle<Value> digitalWrite(const Arguments& args);
Handle<Value> digitalRead(const Arguments& args);

/*#include "gpio.h"

class GPIOWrapper : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:

 GPIO gpio;

  GPIOWrapper();
  ~GPIOWrapper();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> pinMode(const v8::Arguments& args);
  static v8::Handle<v8::Value> digitalWrite(const v8::Arguments& args);
  static v8::Handle<v8::Value> digitalRead(const v8::Arguments& args);
};*/

#endif