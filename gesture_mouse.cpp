
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>

// Global Enum for direction
enum Direction { LEFT = 1, RIGHT = -1, UP = 1, DOWN = -1 };

// Global position variable (initialized to (0, 0))
cv::Point p(0, 0);  // This will store the global position of the cap

// Function to emit virtual input events
void emit(int fd, int type, int code, int value) {
    struct input_event ev = {};
    ev.type = type;
    ev.code = code;
    ev.value = value;
    write(fd, &ev, sizeof(ev));
}

// Set up the uinput device
int setup_uinput_device() {
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("Failed to open /dev/uinput");
        exit(EXIT_FAILURE);
    }

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);

    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);

    struct uinput_setup usetup = {};
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234;
    usetup.id.product = 0x5678;
    strcpy(usetup.name, "Red Cap Mouse");

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);

    sleep(1); // Allow the device to initialize
    return fd;
}

int main() {
    // Open the camera
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the camera!" << std::endl;
        return -1;
    }

    // Setup the virtual input device
    int uinput_fd = setup_uinput_device();

    cv::Point prev_center(0, 0); // Previous center position (initialized to (0, 0))
    bool first_frame = true;  // Flag to check if it's the first frame
    while (true) {
        cv::Mat frame, hsv, mask, result;
        cap >> frame;  // Capture frame from the camera
        if (frame.empty()) {
            std::cerr << "Error: Could not read the frame!" << std::endl;
            break;
        }

        // Convert frame to HSV color space
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        // Define the lower and upper boundaries for red color in HSV space
        cv::Scalar lower_red1(0, 120, 70);
        cv::Scalar upper_red1(10, 255, 255);
        cv::Scalar lower_red2(170, 120, 70);
        cv::Scalar upper_red2(180, 255, 255);

        // Threshold the image to get only red colors
        cv::Mat mask1, mask2;
        cv::inRange(hsv, lower_red1, upper_red1, mask1);
        cv::inRange(hsv, lower_red2, upper_red2, mask2);

        // Combine both masks
        mask = mask1 | mask2;

        // Clean up the mask using morphological operations
        cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

        // Find contours of the red regions
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        cv::Mat black_screen = cv::Mat::zeros(frame.size(), CV_8UC3);

        if (!contours.empty()) {
            // Assume the largest contour corresponds to the cap
            auto max_contour = *std::max_element(contours.begin(), contours.end(),
                                                 [](const std::vector<cv::Point>& c1, const std::vector<cv::Point>& c2) {
                                                     return cv::contourArea(c1) < cv::contourArea(c2);
                                                 });

            // Calculate the bounding box and center
            cv::Rect bounding_box = cv::boundingRect(max_contour);
            cv::Point center(bounding_box.x + bounding_box.width / 2, bounding_box.y + bounding_box.height / 2);

            // Update the direction and global position
            if (!first_frame) {
                int dx = center.x - prev_center.x;  // x-coordinate difference
                int dy = center.y - prev_center.y;  // y-coordinate difference

                // Emit mouse events for relative movement
                emit(uinput_fd, EV_REL, REL_X, dx);
                emit(uinput_fd, EV_REL, REL_Y, dy);
                emit(uinput_fd, EV_SYN, SYN_REPORT, 0);
            } else {
                first_frame = false;
            }

            prev_center = center;

            // Draw the bounding box and center point
            cv::rectangle(black_screen, bounding_box, cv::Scalar(0, 255, 0), 2);
            cv::circle(black_screen, center, 5, cv::Scalar(255, 0, 0), -1);
            // cv::circle(mask, center, 5, cv::Scalar(255, 0, 0), -1);
            
        }

        // Display the result
        // cv::imshow("Frame", frame);
        cv::imshow("Red Mask", mask);

        // Exit on 'q' key press
        if (cv::waitKey(30) == 'q') break;
    }

    // Clean up
    ioctl(uinput_fd, UI_DEV_DESTROY);
    close(uinput_fd);
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
