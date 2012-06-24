/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/*
 *
 * Author: Frank Barwich
 */

#pragma once

#include <string>
#include <vector>

class Compile {
   bool                     debug;
   std::string              outDir;
   std::vector<std::string> includes;
   std::vector<std::string> defines;
   std::vector<std::string> files;
   bool                     crtStatic;
   int                      threads;
   std::string              cc;
   std::string              precompiledHeader;
   std::string              precompiledCpp;

   std::string CommandLine () const;

   void CompilePrecompiledHeaders ();
   void CompileFiles ();

public:
   Compile () : threads(0), debug(false), crtStatic(false) { }

   void Debug (bool debug)                       { this->debug = debug; }
   void OutDir (const std::string& outDir)       { this->outDir = outDir; }
   void AddInclude (const std::string& include)  { includes.push_back(include); }
   void AddDefine (const std::string& define)    { defines.push_back(define); }
   void AddFile (const std::string& file)        { files.push_back(file); }
   void Threads (int threads)                    { this->threads = threads; }
   void CrtStatic (bool crtStatic)               { this->crtStatic = crtStatic; }
   void CC (const std::string& cc)               { this->cc += cc + " "; }
   void PrecompiledHeader (const std::string& v) { precompiledHeader = v; }
   void PrecompiledCpp (const std::string& v)    { precompiledCpp = v; }

   void Go ();
};

