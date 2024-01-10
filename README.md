# 3DViewer v1.0

## Overview

3DViewer is a program developed in C language (C11 standard) with a focus on visualizing 3D wireframe models. The application allows users to load wireframe models from OBJ files, perform translations, rotations, and scaling, and customize the visualization settings. The graphical user interface (GUI) is implemented using the QT framework for Linux and Mac platforms.

## Prerequisites

- C11 compatible compiler (gcc recommended)
- QT framework for GUI components
- Check library for unit testing

## Installation

1. **Clone the Repository:**
   ```
   git clone <repository-url>
   cd C3DViewerApp/src
   ```

2. **Build the Application:**
   ```
   make
   ```

3. **Run the Application:**
   ```
   make run
   ```

## Features

- **Modern UI/UX Design:**
  - Minimalistic and modern web brutalism style design for a visually appealing interface.
  - Intuitive layout for seamless navigation and interaction.

- **Model Manipulation:**
  - Load wireframe models from OBJ files.
  - Translate the model along the X, Y, or Z axes.
  - Rotate the model around its X, Y, or Z axes.
  - Scale the model by a specified value.

- **GUI Interface:**
  - **Model Selection:**
    - Browse and select OBJ files via a button.
    - Display the selected file's name.
  - **Model Transformation:**
    - Translate, rotate, and scale the model using spinbox input fields.
  - **Model Information:**
    - Display details about the uploaded model, including file name, number of vertices, and edges.
  - **Customization:**
    - Customize edge type (solid, dashed), color, and thickness.
    - Choose display method for vertices (none, circle, square), set color and size.
    - Set background color according to preference.

- **Performance:**
  - Handles models with up to 1,000,000 vertices without freezing or interface inactivity.

## Settings

- **Projection Customization:**
  - Choose between parallel and central projection methods.
- **Edge and Vertex Settings:**
  - Set edge type (solid, dashed), color, and thickness.
  - Define display method for vertices (none, circle, square) and set color and size.
- **Background Customization:**
  - Choose the background color for the visualization area.
- **Persistent Settings:**
  - Save customization preferences, including projection type, edge and vertex settings, and background color. Preferences are retained between program restarts.

## Record

- **Image Capture:**
  - Save rendered images as BMP and JPEG files.
- **Screencast Recording:**
  - Record small screencasts demonstrating custom affine transformations of the loaded object into GIF animations. The animations are 640x480 resolution, 10 frames per second, and 5 seconds in duration.

## Build Targets

- `make all`: Build the CLI and GUI versions of 3DViewer.
- `make install`: Install the application in a specified directory.
- `make uninstall`: Remove the installed application.
- `make run`: Run the application.
- `make clean`: Clean build artifacts.

## Developer Notes

- Follow the Google Style Guide for coding standards.
- Ensure full module coverage for model loading and affine transformations with unit tests using the Check library.

## Contributors

- [arseniy-nikitin]
