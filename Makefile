
all:
	docker build -t channel_increment container/
	@docker run --rm -it --volume $(PWD):/opt/src channel_increment --test --compiler gcc
	@docker run --rm -it --volume $(PWD):/opt/src channel_increment --test
