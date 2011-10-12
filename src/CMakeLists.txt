
SET(SOURCE
	main.c
	world.c
	player.c
	simplex.c
	font.c
	game.c
	vec3.c
	util.c
	solver.c
	heightmap.c
	stb_image.c
	textures.c
	materials.c
	glew.c
)

SET(LIBRARIES
	${GLFW_LIBRARIES}
	${OPENGL_LIBRARIES}
	${EXTRA_LIBRARIES}
)

ADD_EXECUTABLE(world-test ${SOURCE})
TARGET_LINK_LIBRARIES(world-test ${LIBRARIES})


