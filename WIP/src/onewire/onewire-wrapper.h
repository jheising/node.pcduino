#ifndef ONEWIRE_WRAPPER_H
#define ONEWIRE_WRAPPER_H

#include <node.h>
#include "onewire.h"

class OneWireWrapper : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:

 OneWire* p_onewire;

  OneWireWrapper(int pinNumber);
  ~OneWireWrapper();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> reset(const v8::Arguments& args);
  static v8::Handle<v8::Value> skip(const v8::Arguments& args);
  static v8::Handle<v8::Value> search(const v8::Arguments& args);
  static v8::Handle<v8::Value> resetSearch(const v8::Arguments& args);
};

#endif