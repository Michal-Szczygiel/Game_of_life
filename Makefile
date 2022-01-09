OUTPUT_FOLDER = FRAMES
FRAMES_NUMBER = 200
FRACTION_ALIVE = 0.2

MOVIE_NAME = game_of_life
FRAME_RATE = 7

build:
	@echo "Skompilowano 'game_of_life'"
	@gcc -O3 main.c -o game_of_life

prepare:
	@sudo apt-get install ffmpeg

run:
	@./game_of_life $(OUTPUT_FOLDER) $(FRAMES_NUMBER) $(FRACTION_ALIVE)
	@ffmpeg -framerate $(FRAME_RATE) -i $(OUTPUT_FOLDER)/frame_%d.bmp -c:v libx264 -strict -2 -preset veryslow \
	-pix_fmt yuv420p -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -f mp4 $(MOVIE_NAME).mp4
