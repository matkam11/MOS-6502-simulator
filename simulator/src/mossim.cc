
#include <cstdlib>
#include <iostream>
#include <sys/time.h>
#include "Terminal.h"
#include "String.h"
#include "Error.h"
#include "Misc.h"

#define VERSION "0.01v"


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

void RegisterOptions()
{
    // Parse the command line and set flags or file output names
}

void ProcessOptions()
{
    // change some configurations based on the flags set
}

// Load programs from context configuration file
void LoadPrograms()
{
        // Load command-line program for when we can actually parse a program
}

void MainLoop()
{
        // Activate signal handler
        //simulation_engine::Engine *esim = esim::Engine::getInstance();

        // Simulation loop
        while (1/*!simulation_engine->hasFinished()*/)
        {
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

                break;
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

        // Process command line
        ProcessOptions();

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
