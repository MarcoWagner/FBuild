/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/*
 *
 * Author: Frank Barwich
 */

#include "FileToCpp.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>


void FileToCpp::CheckParams ()
{
   if (infile.empty()) throw std::runtime_error("Missing 'Infile' for FileToCpp()");
   if (outfile.empty()) throw std::runtime_error("Missing 'Outfile' for FileToCpp()");
   if (nameForArray.empty() && nameForPtr.empty()) throw std::runtime_error("Missing variable name for FileToCpp()");

   if (nameForArray.empty()) nameForArray = nameForPtr + "Array";
   if (nameForPtr.empty()) nameForPtr = nameForArray + "Ptr";

   if (!std::filesystem::exists(infile)) throw std::runtime_error("Missing Infile '" + infile + "' for FileToCpp(). File does not exist.");
}

bool FileToCpp::NeedsRebuild ()
{
   if (!dependencyCheck) return true;
   if (!std::filesystem::exists(outfile)) return true;
   return std::filesystem::last_write_time(infile) > std::filesystem::last_write_time(outfile);
}

void FileToCpp::Create ()
{
   CheckParams();
   if (!NeedsRebuild()) return;
   bool hasNamespace = !nameForNamespace.empty();

   std::ofstream out(outfile, std::ofstream::trunc);
   if (out.fail()) throw std::runtime_error("Error opening " + outfile);

   if (intro.size()) out << intro << "\n\n";
   if (hasNamespace) out << "namespace " << nameForNamespace << " {\n\n";

   if (hasNamespace) out << "   ";
   if (varConst) out << "const ";
   out << "char " << nameForArray << "[] = {\n";

   {
      size_t size = static_cast<size_t>(std::filesystem::file_size(infile));

      boost::interprocess::file_mapping mapping(infile.c_str(), boost::interprocess::read_only);
      boost::interprocess::mapped_region region(mapping, boost::interprocess::read_only, 0, size);

      const char* it = static_cast<const char*>(region.get_address());
      const char* end = it + size;

      out << "      ";

      size_t count = 0;
      std::for_each(it, end - 1, [&] (const char ch) {
         out << (int)ch << ",";
         if (++count == 150) {
            count = 0;
            out << "\n      ";
         }
      });

      out << (int)*(end - 1);
      if (terminatingNull) out << ",0";
      out << "\n";
   }

   if (hasNamespace) out << "   ";
   out << "};\n\n";

   if (hasNamespace) out << "   ";
   if (varConst) out << "const ";
   out << "char* " << nameForPtr << " = " << nameForArray << ";\n\n";

   if (additional.size()) out << additional << "\n\n";

   if (hasNamespace) out << "}\n\n";

   if (outro.size()) out << outro;
   out << "\n\n";
}
