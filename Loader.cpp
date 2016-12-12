//
//  Loader.cpp
//  samples
//
//  Created by apple on 2016/12/11.
//
//

#include <stdio.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace v8;
void Print(const v8::FunctionCallbackInfo<v8::Value>& info){
    v8::String::Utf8Value  v8Str(info[0]);
    printf("%s\n",*v8Str);
}

void Load(const v8::FunctionCallbackInfo<v8::Value>& info){
    v8::String::Utf8Value  v8Str(info[0]);
    std::fstream f(*v8Str);
    std::stringbuf buf;
    f>>&buf;
    f.close();
    
    Script::Compile(v8::String::NewFromUtf8(info.GetIsolate(),buf.str().c_str()))->Run();
    
    
    
}



int main(int argc, char* argv[]) {
    // Initialize V8.
    V8::InitializeICUDefaultLocation(argv[0]);
    V8::InitializeExternalStartupData(argv[0]);
    Platform* platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(platform);
    V8::Initialize();
    // Create a new Isolate and make it the current one.
    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
    v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    Isolate* isolate = Isolate::New(create_params);
    {
        Isolate::Scope isolate_scope(isolate);
        // Create a stack-allocated handle scope.
        HandleScope handle_scope(isolate);
        
        auto global = v8::ObjectTemplate::New(isolate);
        
        
        global->Set(isolate, "print",v8::FunctionTemplate::NewWithFastHandler(isolate, &Print));
        global->Set(isolate, "load",v8::FunctionTemplate::NewWithFastHandler(isolate, &Load));
        // Create a new context.
        Local<Context> context = Context::New(isolate,NULL,global);
        
        
        // Enter the context for compiling and running the hello world script.
        Context::Scope context_scope(context);
        // Create a string containing the JavaScript source code.
        Local<String> source =
        String::NewFromUtf8(isolate, "load('app.js');",
                            NewStringType::kNormal).ToLocalChecked();
        // Compile the source code.
        Local<Script> script = Script::Compile(context, source).ToLocalChecked();
        // Run the script to get the result.
        script->Run(context);
       
        
    }
    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    V8::Dispose();
    V8::ShutdownPlatform();
    delete platform;
    delete create_params.array_buffer_allocator;
    return 0;
}
