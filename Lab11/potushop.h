/***************************************************************************/
/* WARNING: DO NOT CHANGE. This file will be overwritten by Buildbot       */
/***************************************************************************/

#ifndef POTUSHOP_H
#define POTUSHOP_H

/***************************************************************************/
/* Argument Setters                                                        */
/***************************************************************************/

/***************************************************************************/
/* Sets argument for change_brightness() function.                         */
/*                                                                         */
/* - brightness: [-100, 100]                                               */
/***************************************************************************/
void set_brightness_arg(int brightness);

/***************************************************************************/
/* Sets argument for change_brightness() function.                         */
/*                                                                         */
/* - Input Levels:   in_min [0, 253],  in_max [2, 255], in_min < in_max    */
/* - Output Levels: out_min [0, 255], out_max [0, 255]                     */
/***************************************************************************/
void set_level_args(int in_min, int in_max, int out_min, int out_max);

/***************************************************************************/
/* Image Filter Functions                                                  */
/***************************************************************************/

/***************************************************************************/
/* Convert image to grayscale tone                                         */
/*                                                                         */
/* Similar operations in famous image editing software:                    */
/* 1. Gimp: Image > Mode > Grayscale                                       */
/* 2. Photoshop: Image > Mode > Grayscale                                  */
/*                                                                         */
/* - rgb = r * 0.3 + g * 0.59 + b * 0.11                                   */
/***************************************************************************/
void to_grayscale(void);

/***************************************************************************/
/* Convert image to sepia tone                                             */
/*                                                                         */
/* Similar operations in famous image editing software:                    */
/* 1. Gimp: Colors > Desaturate > Sepia...                                 */
/* 2. Photoshop: Image > Adjustments > Hue/Saturation > Preset: Sepia      */
/*                                                                         */
/* r = r * 0.393 + g * 0.769 + b * 0.189                                   */
/* g = r * 0.349 + g * 0.686 + b * 0.168                                   */
/* b = r * 0.272 + g * 0.534 + b * 0.131                                   */
/***************************************************************************/
void to_sepia(void);

/***************************************************************************/
/* Change the brightness of image                                          */
/*                                                                         */
/* Similar operations in famous image editing software:                    */
/* 1. Gimp: Colors > Brightness-Contrast...                                */
/* 2. Photoshop: Image > Adjustments > Brigthness(Legacy)                  */
/*                                                                         */
/* - rgb = rgb + brightness                                                */
/***************************************************************************/
void change_brightness(void);

/***************************************************************************/
/* Change input and output levels of image in order                        */
/*                                                                         */
/* Similar operations in famous image editing software:                    */
/* 1. Gimp: Colors > Levels...                                             */
/* 2. Photoshop: Image > Adjustments > Levels                              */
/*                                                                         */
/* 1. input level:  [min, max] -> [0%, 100%]                               */
/* 2. output level: [0%, 100%] -> [min, max]                               */
/***************************************************************************/
void change_levels(void);

#endif /* POTUSHOP_H */