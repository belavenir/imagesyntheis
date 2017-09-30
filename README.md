##Image forward synthsis test


---


The pipeline of this test is listed in the following:

* We have a RGB image and its depth information, camera calibration information is provided;
* We customize the rigid transformation by euler angles and translation vectors;
* We synthsize the transformed image;
* Note: we simply use a forward process, actually an inverse rendering and bilinear interpolation will loss less pixel, this test is just used to check our rotation function are set up well.


[//]: # (Image References)
[image1]: ./data/rgb.png
[image2]: ./data/depth.png
[image3]: ./data/syn_0.png
[image5]: ./data/syn_pitch0.png
[image4]: ./data/syn_tz1.png
[image6]: ./data/syn_yaw0.png


---


### Test data and results

####1. Test data

Here is the original rgb and its depth map

![alt text][image1]
![alt text][image2]

####2. Results

Without any rotation (some depth are not provided)

![alt text][image3]

A 1m translation away in Z direction

![alt text][image4]

A 10 degree picth change + 1m translation
![alt text][image5]

A 10 degree yaw change + 1m translation
![alt text][image6]


