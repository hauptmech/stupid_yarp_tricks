
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <stdio.h>

using namespace yarp::os;

int main() {
    Network yarp;
    Bottle bot;
    Port input;
    input.open("/read");
    input.read(bot);
    printf("Got message: size:%d, %s\n",bot.size(), bot.toString().c_str());
    input.close();
    return 0;
}
