
/* Author: Traveler Hauptman, (C)2014 
   Adapted from YARP src/libYARP_OS/Companion.cpp
*/

/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011, 2012 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
 * Authors: Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

/*
To add autocompletion see:
http://cc.byexamples.com/2008/06/16/gnu-readline-implement-custom-auto-complete/ 
*/

#include <yarp/conf/system.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>
#include <yarp/os/Stamp.h>
#include <yarp/os/Value.h>
#include <yarp/os/impl/String.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Terminator.h>
#include <yarp/os/Time.h>
#include <yarp/os/Network.h>
#include <yarp/os/Run.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/Property.h>
#include <yarp/os/Ping.h>
#include <yarp/os/YarpPlugin.h>
#include <yarp/os/impl/PlatformStdio.h>
#include <yarp/os/impl/PlatformSignal.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>


using namespace yarp::os;
using namespace std;

static ConstString companion_unregister_name;
static Port *companion_active_port = NULL;
bool adminMode = false;
int done = 0;

/* A static variable for holding the line. */
static char *line_read = (char *)NULL;

/* Read a string, and return a pointer to it.
   Returns NULL on EOF. */
char *
rl_gets ()
{
  /* If the buffer has already been allocated,
     return the memory to the free pool. */
  if (line_read)
    {
      free (line_read);
      line_read = (char *)NULL;
    }

  /* Get a line from the user. */
  line_read = readline (">");

  /* If the line has any text in it,
     save it on the history. */
  if (line_read && *line_read)

    add_history (line_read);

  return (line_read);
}




static void companion_sigint_handler(int sig) {
    double now = Time::now();
    static double firstCall = now;
    static bool showedMessage = false;
    static bool unregistered = false;
    done = 1;
    //sleep(1);
    if (now-firstCall<2) {
        Port *port = companion_active_port;
        if (!showedMessage) {
            showedMessage = true;
            //YARP_LOG_INFO("Interrupting...");
        }
        if (companion_unregister_name!="") {
            if (!unregistered) {
                unregistered = true;
                NetworkBase::unregisterName(companion_unregister_name);
                if (port!=NULL) {
                    NetworkBase::unregisterName(port->getName());
                }
                exit(1);
            }
        }
        if (port!=NULL) {
            port->interrupt();
#ifndef YARP2_WINDOWS
            port->close();
#endif
        }
    } else {
        fprintf(stderr,"Aborting...\n");
        exit(1);
    }
}
static void companion_sigterm_handler(int sig) {
    companion_sigint_handler(sig);
}


static void companion_sighup_handler()
{
    raise(SIGINT);
}

static void companion_install_handler() {
    signal(SIGINT,companion_sigint_handler);
    signal(SIGTERM,companion_sigterm_handler);

    #if defined(WIN32)
    signal(SIGBREAK, (ACE_SignalHandler) companion_sigbreak_handler);
    #else
    signal(SIGHUP, (ACE_SignalHandler) companion_sighup_handler);
    #endif
}



int main(int ntargets, char *targets[]) {
    Network yarp;
    Port port;

    string hist_file(getenv("HOME") );
    hist_file += "/.yarp_write_history";

    read_history(hist_file.c_str());
    if (companion_active_port==NULL) {
        companion_install_handler();
    }
    ntargets--;
    targets++;
    
    if (!port.open(targets[0])) {
        printf("\nCould not open write port.\n");
        return 1;
    }
    ntargets--;
    targets++;

    if (adminMode) {
        port.setAdminMode();
    }

    bool raw = false;
    for (int i=0; i<ntargets; i++) {
        if (string(targets[i])=="verbatim") {
            raw = true;
        } else {
            if (!yarp.connect(port.getName().c_str(),targets[i])) {
			   printf("\nCould not connect ports\n");	
                     return 1; 
            }
        }
    }

	printf("\nTo stop: CTRL+C ENTER\n\n");
	rl_catch_signals = 1;
	rl_set_signals();
    while (!done) {
	    rl_on_new_line();
        string txt(rl_gets());
        if (!done) {
            if (txt[0]<32 && txt[0]!='\n' &&
                txt[0]!='\r' && txt[0]!='\0' && txt[0]!='\t') {
                break;  // for example, horrible windows ^D
            }
            Bottle bot;
            if (raw) {
                bot.addInt(0);
                bot.addString(txt.c_str());
            } else {
                bot.fromString(txt.c_str());
            }
            //core.send(bot);
            port.write(bot);
        }
    }

    write_history(hist_file.c_str());
    companion_active_port = NULL;

    if (raw) {
        Bottle bot;
        bot.addInt(1);
        bot.addString("<EOF>");
        //core.send(bot);
        port.write(bot);
    }

    port.close();

    return 0;
}


