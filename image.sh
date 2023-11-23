#!/bin/sh

convert receipt_novel.pdf[0] -resize 400x -crop 400x150 -type Grayscale -colors 256 image_1.bmp
convert receipt_novel.pdf[1] -resize 400x -crop 400x250 -type Grayscale -colors 256 image_2.bmp
convert receipt_novel.pdf[2] -resize 400x -crop 400x250 -type Grayscale -colors 256 image_3.bmp
convert receipt_novel.pdf[3] -resize 400x -crop 400x250 -type Grayscale -colors 256 image_4.bmp
convert receipt_novel.pdf[4] -resize 400x -crop 400x250 -type Grayscale -colors 256 image_5.bmp
convert receipt_novel.pdf[5] -resize 400x -crop 400x250 -type Grayscale -colors 256 image_6.bmp
convert receipt_novel.pdf[6] -resize 400x -crop 400x350 -type Grayscale -colors 256 image_7.bmp
convert receipt_novel.pdf[7] -resize 400x -crop 400x350 -type Grayscale -colors 256 image_8.bmp

