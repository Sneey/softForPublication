// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This example program shows how to find frontal human faces in an image and
    estimate their pose.  The pose takes the form of 68 landmarks.  These are
    points on the face such as the corners of the mouth, along the eyebrows, on
    the eyes, and so forth.  

    This face detector is made using the classic Histogram of Oriented
    Gradients (HOG) feature combined with a linear classifier, an image pyramid,
    and sliding window detection scheme.  The pose estimator was created by
    using dlib's implementation of the paper:
        One Millisecond Face Alignment with an Ensemble of Regression Trees by
        Vahid Kazemi and Josephine Sullivan, CVPR 2014
    and was trained on the iBUG 300-W face landmark dataset.  

    Also, note that you can train your own models using dlib's machine learning
    tools.  See train_shape_predictor_ex.cpp to see an example.

    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
    instructions.  Note that AVX is the fastest but requires a CPU from at least
    2011.  SSE4 is the next fastest and is supported by most current machines.  
*/


#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------

const string SHAPE_PREDICTOR = "shape_predictor_68_face_landmarks.dat";
//Return vector with names of all jpgs in directory 
std::vector<std::string> getAllJpgFromDir() {
	std::system("dir /B *.jpg > fileS.txt");
	std::vector<std::string> result;
	std::ifstream infile("fileS.txt");
	std::string line;

	while (std::getline(infile, line)) {
		result.push_back(line);
	}
	return result;
}

int main(int argc, char** argv)
{  
    try
    {
        // This example takes in a shape model file and then a list of images to
        // process.  We will take these filenames in as command line arguments.
        // Dlib comes with example images in the examples/faces folder so give
        // those as arguments to this program.
        // We need a face detector.  We will use this to get bounding boxes for
        // each face in an image.
        frontal_face_detector detector = get_frontal_face_detector();
        // And we also need a shape_predictor.  This is the tool that will predict face
        // landmark positions given an image and face bounding box.  Here we are just
        // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
        // as a command line argument.
        shape_predictor sp;
        deserialize(SHAPE_PREDICTOR) >> sp;
		
		std::vector<std::string> listOfPhoto = getAllJpgFromDir();
		
		image_window win, win_faces;
        // Loop over all the images provided on the command line.
		for (int i = 0; i < listOfPhoto.size(); i++)
        {
            cout << "processing image " << listOfPhoto.at(i) << endl;
            array2d<rgb_pixel> img;
            load_image(img, listOfPhoto.at(i));
            // Make the image larger so we can detect small faces.
            pyramid_up(img);

            // Now tell the face detector to give us a list of bounding boxes
            // around all the faces in the image.
            std::vector<rectangle> dets = detector(img);
            cout << "Number of faces detected: " << dets.size() << endl;

            // Now we will go ask the shape_predictor to tell us the pose of
            // each face we detected.
            std::vector<full_object_detection> shapes;
            for (unsigned long j = 0; j < dets.size(); ++j)
            {
                full_object_detection shape = sp(img, dets[j]);

				if (shape.part(30).x() - shape.part(2).x() > 50) cout << "LEFT" << endl;
				if (shape.part(30).x() - shape.part(2).x() < 50) cout << "RIGHT" << endl;
				if (shape.part(30).y() - shape.part(14).y() > -10) cout << "UP" << endl;
				if (shape.part(30).x() - shape.part(14).x() > 10) cout << "DOWN" << endl;
				if (shape.part(30).y() - shape.part(14).y() > 10) cout << "OPENED MOUTH" << endl;
				if (shape.part(44).y() - shape.part(24).y() > 32 && shape.part(37).y() - shape.part(19).y() > 32) cout << "RAISED EYEBROWS" << endl;

				// You get the idea, you can get all the face part locations if
                // you want them.  Here we just store them in shapes so we can
                // put them on the screen.
                shapes.push_back(shape);
            }

            // Now let's view our face poses on the screen.
            win.clear_overlay();
            win.set_image(img);
            win.add_overlay(render_face_detections(shapes));

            // We can also extract copies of each face that are cropped, rotated upright,
            // and scaled to a standard size as shown here:
            dlib::array<array2d<rgb_pixel> > face_chips;
            extract_image_chips(img, get_face_chip_details(shapes), face_chips);
            win_faces.set_image(tile_images(face_chips));

            cout << "Hit enter to process the next image..." << endl;
            std::cin.get();
        }
    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}

// ----------------------------------------------------------------------------------------