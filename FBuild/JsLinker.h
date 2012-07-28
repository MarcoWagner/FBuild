/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/*
 *
 * Author: Frank Barwich
 */

#pragma once

#include "JavaScriptHelper.h"
#include "Linker.h"

class JsLinker : public JavaScriptHelper {
   Linker linker;

   static v8::Handle<v8::Value> GetSet (const v8::Arguments& args);
   static v8::Handle<v8::Value> Link (const v8::Arguments& args);

public:

   static void Register (v8::Handle<v8::ObjectTemplate>& global);
};
