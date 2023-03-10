#include "/home/solindek/.barn/libs/std-cxx/barn_header.hxx"


#include <GL/glut.h>

__BARN_FUNCTION__ int init_gl(std::string window_title, int y_size, int x_size) {
			/* __code__ ./examples/25-opengl-binding.ba */
char** argv = (char**)0;
/* __code__ end */
/* __code__ ./examples/25-opengl-binding.ba */
int argc = 1;
/* __code__ end */
/* __code__ ./examples/25-opengl-binding.ba */
glutInit(&argc, argv);
/* __code__ end */
/* __code__ ./examples/25-opengl-binding.ba */
glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
/* __code__ end */
/* __code__ ./examples/25-opengl-binding.ba */
glutInitWindowPosition(-1,-1);
/* __code__ end */
/* __code__ ./examples/25-opengl-binding.ba */
glutInitWindowSize(y_size, x_size);
/* __code__ end */
/* __code__ ./examples/25-opengl-binding.ba */
glutCreateWindow(window_title.c_str());
/* __code__ end */
	return 1;
}

__BARN_FUNCTION__ void main_loop_gl() {
/* __code__ ./examples/25-opengl-binding.ba */
glutMainLoop();
/* __code__ end */
}

__BARN_FUNCTION__ void register_display_func() {
}

__BARN_FUNCTION__ void swap_buffers_gl() {
/* __code__ ./examples/25-opengl-binding.ba */
glutSwapBuffers();
/* __code__ end */
}

__BARN_FUNCTION__ void clear_default_gl() {
/* __code__ ./examples/25-opengl-binding.ba */
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/* __code__ end */
}

__BARN_FUNCTION__ void clear_color(float r, float g, float b, float a) {
				/* __code__ ./examples/25-opengl-binding.ba */
glClearColor(r, g, b, a);
/* __code__ end */
}

__BARN_FUNCTION__ void display_func() {
	clear_default_gl();
	clear_color((float)255.0, (float)255.0, (float)0.0, (float)255.0);
	swap_buffers_gl();
}

int main() {
	init_gl("OpenGL Barn Biding", 1024, 512);
/* __code__ ./examples/25-opengl-binding.ba */
glutDisplayFunc(display_func);
/* __code__ end */
	main_loop_gl();
	return 0;
}

