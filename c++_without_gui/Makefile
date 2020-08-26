CFLAGS = -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g
LIBS = `pkg-config --libs --cflags gtk+-2.0` `pkg-config --cflags --libs opencv4` -o-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_videoio -lopencv_imgcodecs
CC = g++ -std=c++11
% : %.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) -o $@
