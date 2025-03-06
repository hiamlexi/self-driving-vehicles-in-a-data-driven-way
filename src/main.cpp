#include <cluon-complete-v0.0.127.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "PrimeChecker.hpp"
#include "opendlv-standard-message-set.hpp"

int main(int argc, char **argv) {
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);

    if ((0 == commandlineArguments.count("cid")) || (0 == commandlineArguments.count("name")) || (0 == commandlineArguments.count("width")) ||
        (0 == commandlineArguments.count("height"))) {
        std::cerr << argv[0] << " attaches to a shared memory area containing an ARGB image." << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OD4 session> --name=<name of shared memory area> [--verbose]" << std::endl;
        std::cerr << "         --cid:    CID of the OD4Session to send and receive messages" << std::endl;
        std::cerr << "         --name:   name of the shared memory area to attach" << std::endl;
        std::cerr << "         --width:  width of the frame" << std::endl;
        std::cerr << "         --height: height of the frame" << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=253 --name=img --width=640 --height=480 --verbose" << std::endl;
        return 1;
    }

    // Extract the values from the command line parameters
    const std::string NAME{commandlineArguments["name"]};
    const uint32_t WIDTH{static_cast<uint32_t>(std::stoi(commandlineArguments["width"]))};
    const uint32_t HEIGHT{static_cast<uint32_t>(std::stoi(commandlineArguments["height"]))};
    const bool VERBOSE{commandlineArguments.count("verbose") != 0};

    // Attach to the shared memory.
    std::unique_ptr<cluon::SharedMemory> sharedMemory{new cluon::SharedMemory{NAME}};
    if (sharedMemory && sharedMemory->valid()) {
        std::clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << std::endl;

        // Interface to a running OpenDaVINCI session where network messages are exchanged.
        // The instance od4 allows you to send and receive messages.
        cluon::OD4Session od4{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};

        opendlv::proxy::GroundSteeringRequest gsr;
        std::mutex gsrMutex;
        auto onGroundSteeringRequest = [&gsr, &gsrMutex](cluon::data::Envelope &&env) {
            // The envelope data structure provide further details, such as sampleTimePoint as shown in this test case:
            // https://github.com/chrberger/libcluon/blob/master/libcluon/testsuites/TestEnvelopeConverter.cpp#L31-L40
            std::lock_guard<std::mutex> lck(gsrMutex);
            gsr = cluon::extractMessage<opendlv::proxy::GroundSteeringRequest>(std::move(env));
            std::cout << "lambda: groundSteering = " << gsr.groundSteering() << std::endl;
        };

        od4.dataTrigger(opendlv::proxy::GroundSteeringRequest::ID(), onGroundSteeringRequest);

        // Endless loop; end the program by pressing Ctrl-C.
        while (od4.isRunning()) {
            // OpenCV data structure to hold an image.
            cv::Mat img;

            // Wait for a notification of a new frame.
            sharedMemory->wait();

            // Lock the shared memory.
            sharedMemory->lock();
            {
                // Copy the pixels from the shared memory into our own data structure.
                cv::Mat wrapped(HEIGHT, WIDTH, CV_8UC4, sharedMemory->data());
                img = wrapped.clone();
            }
            sharedMemory->unlock();

            // If you want to access the latest received ground steering, don't forget to lock the mutex:
            {
                std::lock_guard<std::mutex> lck(gsrMutex);
                std::cout << "main: groundSteering = " << gsr.groundSteering() << std::endl;
            }

            // Display image on your screen.
            if (VERBOSE) {
                cv::imshow(sharedMemory->name().c_str(), img);
                cv::waitKey(1);
            }
        }
    }

    return 0;
}
