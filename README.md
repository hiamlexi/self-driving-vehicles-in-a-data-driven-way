# self-driving-vehicles-in-a-data-driven-way

![pipeline badge](https://git.chalmers.se/courses/dit638/students/2024-group-12/badges/main/pipeline.svg)
The functional requirements drove our algorithmic and system design, ensuring the software meets its goals and is maintainable. Our real-time object detection pipeline processes each frame to identify cones (yellow, orange, and blue) crucial for steering angle calculation. The pipeline applies color filtering (using HSV ranges), bilateral filtering for noise reduction, and contour detection—leveraging the Douglas-Peucker algorithm and additional criteria—to accurately identify valid cone candidates within a defined region of interest (RoI).

Initially, we considered using a KCF tracker for object tracking, but concerns over resource costs and other limitations, combined with the clear visibility of cones, led us to discard it.

For steering angle calculation, we first attempted to derive the angle from bounding box positions. However, due to inconsistent results, we shifted to a data-driven approach. By correlating angular velocity readings with steering angles, we used MATLAB’s polyfit function for polynomial regression, ultimately refining a 4-degree polynomial equation that achieved a stable success rate of approximately 33%.

The system is deployed on a microservices-based architecture consisting of two nodes—a host node (running Docker and H264 decoder) and a virtual machine node (running our application and web browser)—communicating via shared memory.
![image](https://github.com/user-attachments/assets/659760d4-cc68-40fc-9e10-2bcdd5db02a7)

This diagram shows our system works utilizing a microservices-based architecture and how our application integrates with this pre-provided architecture cohesively.
![image](https://github.com/user-attachments/assets/3a7b0005-c50c-46ba-a97a-dd9b120604b9)

Decode frame by using h264decoder micro service. This copying into the shared memory area works as follows: Firstly, the h264decoder tries to lock the shared memory for exclusive access (similar to a mutexLinks to an external site.); if any other process would hold the exclusive access to the shared memory, the h264decoder is blocked and waits until the access is possible. If the access is possible, the shared memory area is locked and the pixel data is decoded from the raw h264 frame into the shared memory area. Afterwards, the shared memory area is unlocked and all other processes, which are currently sleeping while waiting for a notification to continue working with new data in the shared memory environment, will receive a notification signal from h264decoder. Our microservice my-opencv-example is acting as consumer for new data and is usually waiting for new data to arrive. The microservice is attaching to an existing shared memory area, which in our example is created by h264decoder and falls asleep to wait for a notification from h264decoder to continue. Once our microservice is receiving this notification, it is waking up, locking the shared memory area and from that moment on, it can do something with the pixel data; in our example, it is creating an OpenCV data structure from the pixel data in the shared memory environment to overlay a red rectangle and display the frame.

![image](https://github.com/user-attachments/assets/ddc1f10c-7342-45f8-813c-c4d0f3442426)
https://github.com/user-attachments/assets/63363982-3f39-4a8d-b952-daa3a67636b4

Detecting objects in images is a popular task for computer visionLinks to an external site., where recently many remarkable results have been achieved, especially thanks to neural networks. The drawback of such approaches, however, is that they usually are computationally expensive. Embedded systems such as the Kiwi miniature car, however, typically do not have powerful GPUs where we can run complex neural networks. Instead, much more lean approaches such as segmenting objects by separating the color can be an easier solution.
![image](https://github.com/user-attachments/assets/9dae865c-04e2-464f-a84a-0c05ccfec61e)
In the example above you see a demo on how to use the OpenDLV microservice for decoding video frames in combination with the OpenDLV microservice to experiment with various settings for the HSV parameters to easily segment objects by color properties.

https://github.com/user-attachments/assets/80f371dc-1b16-459c-86cd-d42c0edd060a


## Dependencies




### System requirements
Ubuntu 22.04

### Tools
* build-essential
* cmake
* git
* Docker

## Getting started

Clone the repository
```

```

Next, to build the project, create a build directory and run these commands to build the project
```
mkdir build
cd build
cmake ..
make
```

Subsequent builds only require you to run the ``make`` command, unless there have been changes to any of the CMake files

In order to build and run the tests, use the following
```
make test
```

## Way of working

We intend to use the git feature branch work flow, where we create a new branch for each issue which represents a single feature, piece of work or fix. Each feature branch is required to go through a code review by opening a merge request by the author, assigning another member for a review.
Code review comments are optional, the code review may either leave a comment or contact the author for further discussion on changes or improvements

### Commit messages

Commit messages are required to reference the issue number in the subject of the commit message.
The subject of the commit message is required, while the body is optional.

Template: 
```
#[issue ID]: [subject]

[optional body]
```

Example commit message for issue with id 12
```
#12: added more test coverage
```

## License
MIT License
