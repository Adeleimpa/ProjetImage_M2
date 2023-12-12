README

Prepare data

Download the celebA database from the website https://mmlab.ie.cuhk.edu.hk/projects/CelebA.html.
Run the script "data_convert.py". This process will convert the images from jpg to ppm format in a new folder.
Keep the first 10,000 images from the newly created folder. This will constitute the dataset you will use.
In the newly created folder, rename the image "010000.ppm" to "000000.ppm".
In the file list_landmarks_align_celeba.txt, find the line corresponding to the image "010000.jpg". Rename this image to "000000.jpg" and ensure that this line is placed before the entry for the image "000001.jpg".
In the file list_attr_celeba.txt, find the line corresponding to the image 010000.jpg. Rename this image to 000000.jpg, and ensure that this line is placed before the entry for the image 000001.jpg.

How to run ?

0. open terminal

1.choose the path of your main.cpp file
cd <path>

2. compile
g++ -std=c++11 main.cpp -o run

3. run
./run A 000001.jpg 1_inverse_gender.ppm 1_swap.ppm
