/*
 *  Multi2Sim
 *  Copyright (C) 2013  Rafael Ubal (ubal@ece.neu.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <cstdlib>
#include <iostream>
#include <sys/time.h>
#include <fstream>
#include <iomanip>

#include "Terminal.h"
#include "String.h"
#include "Error.h"
#include "Misc.h"
#include "6502.h"
#include "CommandLine.h"

#define VERSION "0.01v"

//
// Configuration options
//

// Binary file for OpenCL runtime
std::string mossim_binary_path;


bool binary_hex_dump=0;


void WelcomeMessage(std::ostream &os)
{

    // Compute simulation ID
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned min_id = misc::StringAlnumToInt("10000");
    unsigned max_id = misc::StringAlnumToInt("ZZZZZ");
    unsigned id = (tv.tv_sec * 1000000000 + tv.tv_usec)
                    % (max_id - min_id + 1)
                    + min_id;
    std::string alnum_id = misc::StringIntToAlnum(id);

    // Blue color
    misc::Terminal::Blue(os);

        // Print welcome message in the standard error output
        os << '\n' << "; Mos 6502 Simulator " << VERSION << " \n";
        os << "; Please use command 'mossim --help' for a list of command-line options.\n";
        os << "; Simulation alpha-numeric ID: " << alnum_id << '\n';
        os << '\n';
        // Reset terminal color
        misc::Terminal::Reset(os);

}

// Load a program from the command line
void LoadProgram(const std::vector<std::string> &arguments,
                const std::vector<std::string> &environment = {},
                const std::string &current_directory = "",
                const std::string &stdin_file_name = "",
                const std::string &stdout_file_name = "")
{
        // Skip if no program specified
        if (arguments.size() == 0)
                return;

        // Choose emulator based on ELF header
        std::string exe = misc::getFullPath(arguments[0], current_directory);
        std::cout<<"Input file: "<< exe<<std::endl;

        uint16_t i = Emulator::getInstance().GetBaseAddr();

        char byte;
        std::ifstream inputBinary;
        inputBinary.open(exe, std::ios::in | std::ios::binary);
        while (inputBinary.good()) {
            inputBinary.read(&byte,1);
            //inputBinary >> std::setw(2) >> std::setprecision(2) >> std::hex >> byte;
            Emulator::getInstance().WriteMem(i,byte);
            i++;
        }

        if(binary_hex_dump)
            Emulator::getInstance().PrintMem();


        inputBinary.close();

}

// Load programs from context configuration file
void LoadPrograms()
{
        // Load command-line program
        misc::CommandLine *command_line = misc::CommandLine::getInstance();
        LoadProgram(command_line->getArguments());
}

void RegisterOptions()
{
    // Parse the command line and set flags or file output names

    // Set error message
    misc::CommandLine *command_line = misc::CommandLine::getInstance();
    command_line->setErrorMessage("\nPlease type 'm2s --help' for a list of "
                    "valid Multi2Sim command-line options.");

    // Set help message
    command_line->setHelp("Syntax:"
                    "\n\n"
                    "$ mossim [<options>] [<exe>]"
                    "\n\n"
                    "MosSim's command line can take a program "
                    "executable <exe> as an argument, given as a binary "
                    "file in any of the supported CPU architectures, and "
                    "optionally followed by its arguments <args>. The "
                    "following list of command-line options can be used "
                    "for <options>:");

    // Help message for memory system
    command_line->RegisterBool("--binary-hex-dump",
                    binary_hex_dump,
                    "Print the binary file in hex format"
                    );

}

void ProcessOptions()
{
    // change some configurations based on the flags set
}

// Load programs from context configuration file
// void LoadPrograms()
// {
//         // Load command-line program for when we can actually parse a program
// }

void MainLoop()
{

        // Simulation loop
        while (1/*!simulation_engine->hasFinished()*/)
        {
            if (!Emulator::getInstance().Decode()) {
                break;
            }
                // =============================================================
                // Event-driven simulation. Only process events and advance to
                // next global simulation cycle if any architecture performed a
                // useful timing simulation.
                // if (num_active_timing_simulators)
                //	esim->ProcessEvents();


                // =============================================================
                // If neither functional nor timing simulation was performed for
                // any architecture, it means that all guest contexts finished
                // execution - simulation can end.
                // if (!still_running)
                // 	simulation_engine->Finish("ContextsFinished");

                // =============================================================
                // Count loop iterations, and check for limit in simulation time
                // only every 128k iterations. This avoids a constant overhead
                // of system calls.

                // loop_iterations++;
                // if (m2s_max_time > 0
                //		&& !(loop_iterations & ((1 << 17) - 1))
                //		&& simulation_engine->getRealTime() > max_time * 1000000)
                //	simulation_engine->Finish("MaxTime");

        }

        // Process all remaining events
        // simulation_engine->ProcessAllEvents();
        std::cout<<"Simulation Finished!"<<std::endl;

}

int MainProgram(int argc, char **argv)
{
        // Print welcome message in standard error output
        WelcomeMessage(std::cerr);

        // Read command line
        RegisterOptions();
        Emulator::getInstance().RegisterOptions();

        // Process command line
        misc::CommandLine *command_line = misc::CommandLine::getInstance();
        command_line->Process(argc, argv, false);

        // Process command line
        ProcessOptions();
        Emulator::getInstance().ProcessOptions();

        // Load programs
        LoadPrograms();

        // Main simulation loop
        MainLoop();

        // Success
        return 0;
}

int main(int argc, char **argv)
{
        // Main exception handler
        try
        {
                // Run main program
                return MainProgram(argc, argv);
        }
        catch (misc::Exception &e)
        {
                e.Dump();
                return 1;
        }
}
