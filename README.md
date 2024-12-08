# Hand Gensture-Controlled Mouse Driver 🎮🖱️
This project implements a gesture-controlled mouse driver using computer vision, where hand gestures wearing red identifiers on 1st three fingers act as input to control the mouse cursor. It leverages real-time camera feeds to track hand movements and map them to mouse actions such as movement, clicking, and more.

### Features ✨
- **Real-Time Hand Tracking:** Detects and tracks red caps on hands using OpenCV.
- **Gesture-Based Control:** Move the cursor, click, or perform other mouse operations by moving your hands.
- **Customizable Tracking:** Adjust thresholds for better red cap detection under different lighting conditions.
- **Global Position Tracking:** Tracks cumulative movement over frames, displaying the current global position.
- **Interactive Visualization:** Real-time feedback with bounding boxes and center points overlaid on the video feed.

### How It Works 🛠️
- Wear red identifier on your fingers or hands.
- The webcam detects the caps by isolating red regions in the video feed using HSV color segmentation.
- The program calculates positional changes (dx, dy) between frames and updates the mouse cursor's position.
- Movements are mapped to directional inputs:
    - Left/Right: Horizontal movements.
    - Up/Down: Vertical movements.
- A secondary display shows the global position of the cursor.

### Requirements 🖥️
- OpenCV 4.x
- C++17 or higher
- A webcam or camera-enabled device
- A red mark on finger (by red tape red-colored marker)

### Usage 🚀
- Clone the repository:
    ``` copy
    git clone https://github.com/AS-0167/red-cap-gesture-mouse.git
    ```    

- Build the project:
    ``` copy
    sudo g++ gesture_mouse.cpp -o gesture_mouse -I/usr/include/opencv4 -L/usr/lib/x86_64-linux-gnu -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio
    ```

- Run the program:
    ``` copy
    ./gesture_mouse
    ```

### Customization ⚙️
- Modify the HSV thresholds in the source code to adjust for lighting variations.
- Tune contour area thresholds to optimize cap detection.

### Applications 📈
- Hands-free mouse control for accessibility.
- Gesture-based gaming and interactive experiences.
- Robotics and automation where precise hand gesture inputs are needed.

### Contribution 🤝
Contributions are welcome! Submit a pull request or open an issue for bug reports and feature suggestions.
