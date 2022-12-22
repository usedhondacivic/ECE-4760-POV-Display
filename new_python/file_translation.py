from PIL import Image
import sys
import numpy
from matplotlib import pyplot as plt


NUM_LEDS = 40
DIVISIONS_PER_ROTATION = 120

numpy.set_printoptions(threshold=sys.maxsize)


def translate_image(path):
    # Note: a ton of these operations can be done in slick numpy one liners, but I'm too dumb and want to be able to read my code
    # Also if I wanted stupid fast code I wouldn't be using python
    img_array_raw = Image.open(path)
    img_array_np = numpy.asarray(img_array_raw)

    width, height, channels = img_array_np.shape

    min_dim = min(width, height)

    temp_img = numpy.zeros((width, height, 3))

    # The image may have alpha, which can't be displayed on LEDs
    # Our analogue is turning the LED off, so we dim the RGB channels according to the alpha value
    # Todo: Check that this is inline with how our eyes work. Is proportional scaling of the channels actually valid?
    if channels == 4:
        for w in range(width):
            for h in range(height):
                temp_img[w][h] = img_array_np[w][h][0: 3] * \
                    (img_array_np[w][h][3] / 255)

        img_array_np = temp_img.astype(int)

    rad_img = numpy.zeros((DIVISIONS_PER_ROTATION, NUM_LEDS, 3))

    # Sample the image at each point that an LED will update at
    # We calculate this point in polar space, convert it to rectangular, then sample the image at that point
    # The results are stored in rad_img, which is a polar representation. Essentially (theta, r)
    for t in range(DIVISIONS_PER_ROTATION):
        for l in range(NUM_LEDS):
            # What angle are we looking at (in radians)
            theta = t * ((2 * numpy.pi) / DIVISIONS_PER_ROTATION)
            # How far out are we (from 0 to 1)
            r = l / NUM_LEDS

            x_raw = numpy.cos(theta) * r
            y_raw = numpy.sin(theta) * r

            # Get the rectangular coord for the current polar coord, centered on the image and going to the edges
            x = numpy.interp(
                x_raw, [-1, 1], [(width / 2) - (min_dim / 2), (width / 2) + (min_dim / 2)])
            y = numpy.interp(
                y_raw, [-1, 1], [(height / 2) - (min_dim / 2), (height / 2) + (min_dim / 2)])

            x = int(x)
            y = int(y)

            assert x < width and x > 0
            assert y < height and y > 0

            rad_img[t][l] = img_array_np[x][y]

    rad_img = rad_img.astype(int)

    return rad_img


def translate_gif(path):
    pass


def translate_video(path, start_frame, end_frame):
    pass


def visualize_data(data):
    x = numpy.zeros(DIVISIONS_PER_ROTATION * NUM_LEDS)
    y = numpy.zeros(DIVISIONS_PER_ROTATION * NUM_LEDS)
    color = numpy.zeros((DIVISIONS_PER_ROTATION * NUM_LEDS, 3))

    for t in range(DIVISIONS_PER_ROTATION):
        for l in range(NUM_LEDS):
            theta = t * ((2 * numpy.pi) / DIVISIONS_PER_ROTATION)
            # How far out are we (from 0 to 1)
            r = l / NUM_LEDS

            x[t * NUM_LEDS + l] = numpy.cos(theta) * r
            y[t * NUM_LEDS + l] = numpy.sin(theta) * r

            color[t * NUM_LEDS + l] = data[t][l] / 255

    plt.scatter(y, x, c=color)
    plt.show()


if __name__ == '__main__':
    data = translate_image('./bird.png')
    visualize_data(data)
