//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/
#include <algorithm>

// remember to prototype
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );


typedef vec4 point4;
typedef vec4 color4;
using namespace std;


const int FPS = 60;
const int FRAME_TIME = 1000 / FPS;

mat4 projection;
GLuint projection_loc;
mat4 view;
GLuint view_loc;
mat4 model;
GLuint model_loc;

mat4 instance;

mat4 model_Moc;

float ToaDo_Moc[3] = { 0,0,0 };
float ToaDo_ThungHang[3] = { 0,0,0 };
float ToaDo_XeTai[3] = { 0,0,0 };
mat4 model_ThungHang; // TH bỏ móc hàng
bool is_Moc = false, is_Moved = false; // Kiểm tra hàng được móc chưa

vec4 tranformed;
vec3 final_vertex;


point4 light_position(10, 20, 20, 0.0);
color4 light_ambient(0.4, 0.4, 0.4, 1.0);
color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
color4 light_specular(1.0, 1.0, 1.0, 1.0);


color4 ambient_product;
color4 diffuse_product;
color4 specular_product;

color4 material_ambient(1.0, 0.0, 1.0, 1.0);
color4 material_diffuse(0.4f, 0.4f, 0.4f, 1.0f);
color4 material_specular(0.774597f, 0.774597f, 0.774597f, 1.0f);
float material_shininess = 80.0f;


// LẬP PHƯƠNG
// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[NumPoints];

point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/


void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int Index = 0;
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

// TRỤ TAM GIÁC

const int NumPoints2 = 24;

point4 points2[NumPoints2];
color4 colors2[NumPoints2];
vec3 normals2[NumPoints2];

point4 vertices2[6];
point4 vertices2_colors[6];

void initTruTG() {
	vertices2[0] = point4(0.0, -0.5, -0.5, 1.0);
	vertices2[1] = point4(0.0, 0.5, -0.5, 1.0);
	vertices2[2] = point4(0.5, 0.5, 0.0, 1.0);
	vertices2[3] = point4(0.5, -0.5, 0.0, 1.0);
	vertices2[4] = point4(-0.5, -0.5, 0.0, 1.0);
	vertices2[5] = point4(-0.5, 0.5, 0.0, 1.0);

	vertices2_colors[0] = color4(0.0, 0.0, 0.0, 1.0);
	vertices2_colors[1] = color4(1.0, 0.0, 0.0, 1.0);
	vertices2_colors[2] = color4(1.0, 1.0, 0.0, 1.0);
	vertices2_colors[3] = color4(0.0, 1.0, 0.0, 1.0);
	vertices2_colors[4] = color4(1.0, 0.0, 1.0, 1.0);
	vertices2_colors[5] = color4(0.0, 1.0, 1.0, 1.0);
}

void quad2(int a, int b, int c, int d) {
	vec4 u = vertices2[b] - vertices2[a];
	vec4 v = vertices2[c] - vertices2[b];
	vec3 normal = normalize(cross(v, u));

	normals2[Index] = normal; colors2[Index] = vertices2_colors[a]; points2[Index] = vertices2[a]; Index++;
	normals2[Index] = normal; colors2[Index] = vertices2_colors[b]; points2[Index] = vertices2[b]; Index++;
	normals2[Index] = normal; colors2[Index] = vertices2_colors[c]; points2[Index] = vertices2[c]; Index++;
	normals2[Index] = normal; colors2[Index] = vertices2_colors[a]; points2[Index] = vertices2[a]; Index++;
	normals2[Index] = normal; colors2[Index] = vertices2_colors[c]; points2[Index] = vertices2[c]; Index++;
	normals2[Index] = normal; colors2[Index] = vertices2_colors[d]; points2[Index] = vertices2[d]; Index++;
}

void makeColorTruTG() {
	quad2(1, 0, 3, 2);
	quad2(5, 4, 0, 1);
	quad2(3, 4, 5, 2);

	points2[18] = vertices2[0];
	points2[19] = vertices2[4];
	points2[20] = vertices2[3];

	points2[21] = vertices2[1];
	points2[22] = vertices2[2];
	points2[23] = vertices2[5];

	colors2[18] = vertices2_colors[0];
	colors2[19] = vertices2_colors[4];
	colors2[20] = vertices2_colors[3];

	colors2[21] = vertices2_colors[1];
	colors2[22] = vertices2_colors[2];
	colors2[23] = vertices2_colors[5];

	vec4 uTop = vertices2[0] - vertices2[4];
	vec4 vTop = vertices2[3] - vertices2[0];
	vec3 normalTop = normalize(cross(uTop, vTop));

	for (int i = 18; i < 21; i++) {
		normals2[i] = normalTop;
	}

	vec4 uBot = vertices2[1] - vertices2[2];
	vec4 vBot = vertices2[5] - vertices2[1];
	vec3 normalBot = normalize(cross(uBot, vBot));

	for (int i = 21; i < 24; i++) {
		normals2[i] = normalBot;
	}
}

// TỨ DIỆN

const int NumPoints3 = 12;
point4 points3[NumPoints3] = {
	point4(-0.5, 0.0, -0.5, 1.0),
	point4(0.5, 0.0, -0.5, 1.0),
	point4(0.0, 0.5, -0.5, 1.0),

	point4(0.0, 0.5, -0.5, 1.0),
	point4(0.5, 0.0, -0.5, 1.0),
	point4(0.0, 0.0, 0.0, 1.0),

	point4(0.0, 0.0, 0.0, 1.0),
	point4(-0.5, 0.0, -0.5, 1.0),
	point4(0.0, 0.5, -0.5, 1.0),

	point4(0.0, 0.0, 0.0, 1.0),
	point4(0.5, 0.0, -0.5, 1.0),
	point4(-0.5, 0.0, -0.5, 1.0),
};

color4 colors3[NumPoints3] = {
	color4(1.0, 0.0, 0.0, 1.0),
	color4(0.0, 1.0, 0.0, 1.0),
	color4(0.4, 0.0, 0.6, 1.0),

	color4(0.4, 0.0, 0.6, 1.0),
	color4(0.0, 1.0, 0.0, 1.0),
	color4(0.2, 0.3, 0.0, 1.0),


	color4(0.2, 0.3, 0.0, 1.0),
	color4(1.0, 0.0, 0.0, 1.0),
	color4(0.4, 0.0, 0.6, 1.0),

	color4(0.2, 0.3, 0.0, 1.0),
	color4(0.0, 1.0, 0.0, 1.0),
	color4(1.0, 0.0, 0.0, 1.0),
};
vec3 normals3[NumPoints3] = {
	normalize(cross(points3[1] - points3[0], points3[0] - points3[2])),
	normalize(cross(points3[1] - points3[0], points3[0] - points3[2])),
	normalize(cross(points3[1] - points3[0], points3[0] - points3[2])),

	normalize(cross(points3[4] - points3[3], points3[3] - points3[5])),
	normalize(cross(points3[4] - points3[3], points3[3] - points3[5])),
	normalize(cross(points3[4] - points3[3], points3[3] - points3[5])),

	normalize(cross(points3[6] - points3[8], points3[8] - points3[7])),
	normalize(cross(points3[6] - points3[8], points3[8] - points3[7])),
	normalize(cross(points3[6] - points3[8], points3[8] - points3[7])),

	normalize(cross(points3[11] - points3[10], points3[10] - points3[9])),
	normalize(cross(points3[11] - points3[10], points3[10] - points3[9])),
	normalize(cross(points3[11] - points3[10], points3[10] - points3[9])),

};

// TRU TRON
const int NumPoints4 = 100;

point4 points4[2 * (NumPoints4 + 2) + 2 * (NumPoints4 + 1)];
color4 colors4[2 * (NumPoints4 + 2) + 2 * (NumPoints4 + 1)];
vec3 normals4[2 * (NumPoints4 + 2) + 2 * (NumPoints4 + 1)];
GLfloat radius = 0.5f; // Ban kinh

void updateFrame(int value)
{
	glutPostRedisplay(); // Cập nhật khung hình 
	glutTimerFunc(FRAME_TIME, updateFrame, 0); // Gọi lại hàm updateFrame sau khoảng FRAME_TIME milisecond
}

void initTT() {
	GLfloat angleStep = 360.0f / NumPoints4;

	points4[0] = point4(0.0, 0.5, 0.0, 1.0);
	colors4[0] = color4(1.0, 0.0, 0.0, 1.0);
	normals4[0] = vec3(0, 1, 0);

	points4[NumPoints4 + 2] = point4(0.0, -0.5, 0.0, 1.0);
	colors4[NumPoints4 + 2] = color4(0, 1, 0, 1.0);
	normals4[NumPoints4 + 2] = vec3(0, -1, 0);

	for (int i = 1; i <= NumPoints4 + 1; ++i) {
		GLfloat x = radius * cos(DegreesToRadians * angleStep * i);
		GLfloat z = radius * sin(DegreesToRadians * angleStep * i);
		points4[i] = point4(x, 0.5, z, 1.0);
		colors4[i] = color4(1, 0, 0, 1.0);
		normals4[i] = vec3(0, 1, 0);


		points4[i + NumPoints4 + 2] = point4(x, -0.5, z, 1.0);
		colors4[i + NumPoints4 + 2] = color4(0, 1, 0, 1.0);
		normals4[i + NumPoints4 + 2] = vec3(0, -1, 0);
	}
	for (int i = 2 * (NumPoints4 + 2); i < 2 * (NumPoints4 + 2) + 2 * (NumPoints4 + 1); i++) {
		GLfloat x = radius * cos(DegreesToRadians * angleStep * i);
		GLfloat z = radius * sin(DegreesToRadians * angleStep * i);
		if (i % 2 == 0) {
			points4[i] = point4(x, 0.5, z, 1.0);
			colors4[i] = color4(1, 0, 0, 1.0);
		}
		else {
			points4[i] = point4(x, -0.5, z, 1.0);
			colors4[i] = color4(0, 1, 0, 1.0);
		}
	}

	for (int i = 2 * (NumPoints4 + 2); i < 2 * (NumPoints4 + 2) + 2 * (NumPoints4 + 1); i++) {

		if (i == 2 * (NumPoints4 + 2) + 2 * (NumPoints4 + 1) - 2) {
				normals4[i] = normalize(cross(points4[i-1] - points4[i], points4[i] - points4[i + 1]));
		} else if (i == 2 * (NumPoints4 + 2) + 2 * (NumPoints4 + 1) - 1) {
			normals4[i] = normalize(cross(points4[i] - points4[i-1], points4[i-1] - points4[2 * (NumPoints4 + 2)]));
		}
		else {

			if (i % 2 == 0)
				normals4[i] = normalize(cross(points4[i] - points4[i + 2], points4[i + 2] - points4[i + 1]));
			else
				normals4[i] = normalize(cross(points4[i] - points4[i - 1], points4[i - 1] - points4[i + 1]));
		}
	}
}


void generateGeometry( void )
{
	initCube();
	makeColorCube();
	Index = 0;
	initTruTG();
	makeColorTruTG();
	initTT();
}



GLuint program;


void initGPUBuffers( void )
{
	// Tạo một VAO - vertex array object
	GLuint vao;
    glGenVertexArrays( 1, &vao );     
    glBindVertexArray( vao );

    // Tạo và khởi tạo một buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(points2) + sizeof(colors2) + sizeof(points3) + sizeof(colors3) + sizeof(points4) + sizeof(colors4) + sizeof(normals) + sizeof(normals2) + sizeof(normals3) + sizeof(normals4), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(points2), points2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(points2), sizeof(points3), points3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(points2) + sizeof(points3), sizeof(points4), points4);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(points2) + sizeof(points3) + sizeof(points4), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(points2) + sizeof(points3) + sizeof(points4) + sizeof(colors), sizeof(colors2), colors2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(points2) + sizeof(points3) + sizeof(points4) + sizeof(colors) + sizeof(colors2), sizeof(colors3), colors3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(points2) + sizeof(points3) + sizeof(points4) + sizeof(colors) + sizeof(colors2) + sizeof(colors3), sizeof(colors4), colors4);
	
	
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(points2) + sizeof(points3) + sizeof(points4) + sizeof(colors) + sizeof(colors2) + sizeof(colors3) + sizeof(colors4), sizeof(normals), normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(points2) + sizeof(points3) + sizeof(points4) + sizeof(colors) + sizeof(colors2) + sizeof(colors3) + sizeof(colors4) + sizeof(normals), sizeof(normals2), normals2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(points2) + sizeof(points3) + sizeof(points4) + sizeof(colors) + sizeof(colors2) + sizeof(colors3) + sizeof(colors4) + sizeof(normals) + sizeof(normals2), sizeof(normals3), normals3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(points2) + sizeof(points3) + sizeof(points4) + sizeof(colors) + sizeof(colors2) + sizeof(colors3) + sizeof(colors4) + sizeof(normals) + sizeof(normals2) + sizeof(normals3), sizeof(normals4), normals4);

}



void SetUpColor() {
	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}

void shaderSetup( void )
{
	// Nạp các shader và sử dụng chương trình shader
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );   // hàm InitShader khai báo trong Angel.h
    glUseProgram( program );

    // Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
    GLuint loc_vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( loc_vPosition );
    glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(points2) + sizeof(points3) + sizeof(points4)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(points2) + sizeof(points3) + sizeof(points4) + sizeof(colors) + sizeof(colors2) + sizeof(colors3) + sizeof(colors4)));

	SetUpColor();

	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);

    glClearColor( 1.0, 1.0, 1.0, 1.0 );        /* Thiết lập màu trắng là màu xóa màn hình*/
}

// Màu sắc chiếu sáng

// GG: material for opengl -> free.fr

void ChangeColor(float a, float b, float c, float x, float y, float z, float q, float w, float e, float light) {
	material_ambient = color4(a, b, c, 1);
	material_diffuse = color4(x, y, z, 1);
	material_specular = color4(q, w, e, 1);
	material_shininess = light;
	SetUpColor();
}

// Bánh Xe
void BanhXe(mat4 model_phancap) {
	ChangeColor(0.02, 0.02, 0.02, 0.01, 0.01, 0.01, 0.52, 0.52, 0.52, 78);
	instance = RotateX(90) * Scale(0.5, 0.2, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_phancap * instance);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));

	ChangeColor(0.19225f, 0.19225f, 0.19225f, 0.50754f, 0.50754f, 0.50754f, 0.52, 0.52, 0.52, 78);
	instance = RotateX(90) * Scale(0.3, 0.21, 0.3);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_phancap * instance);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));
}

void DenXe(GLfloat x, GLfloat y, GLfloat z, mat4 model_Xe) {
	ChangeColor(0.6, 0.5, 0, 0.6, 0.4, 0, 0.52, 0.52, 0.52, 78);
	instance = Translate(x, y, z) * Scale(0.02, 0.1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

// Đầu Xe
void DauXeDuoi(mat4 model_Xe) {

	ChangeColor(0.6, 0.1, 0, 1, 0.2, 0, 0.52, 0.52, 0.52, 78);
	instance = Translate(0.3, 0.42, 0) * Scale(0.4, 0.2, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	instance = Translate(-0.2, 0.42, 0) * Scale(0.6, 0.2, 0.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void KinhTruoc(mat4 model_Xe) {
	ChangeColor(0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 0.52, 0.52, 0.52, 78);
	instance = Translate(0.482, 1.02, 0) * Scale(0.04, 0.56, 0.96);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void KinhBen(GLfloat x, GLfloat y, GLfloat z, mat4 model_Xe) {
	ChangeColor(0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 0.52, 0.52, 0.52, 78);
	instance = Translate(x, y, z) * Scale(0.4, 0.4, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void DauXeTren(mat4 model_Xe) {

	ChangeColor(0.6, 0.1, 0, 1, 0.2, 0, 0.52, 0.52, 0.52, 78);
	instance = Translate(0, 0.92, 0) * Scale(1, 0.8, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	KinhTruoc(model_Xe);
	KinhBen(0.2, 0.95, -0.482, model_Xe);
	KinhBen(0.2, 0.95,0.482, model_Xe);

	DenXe(0.5, 0.45, 0.4, model_Xe);
	DenXe(0.5, 0.45, -0.4, model_Xe);

	DenXe(-3.5, 0.45, 0.4, model_Xe);
	DenXe(-3.5,0.45, -0.4, model_Xe);
}

void DauXe(mat4 model_Xe) {
	DauXeTren(model_Xe);
	DauXeDuoi(model_Xe);
}

// Thân Xe
void DuoiXe(mat4 model_Xe) {

	ChangeColor(0.2, 0.2, 0.2, 0.4, 0.4, 0.4, 0.52, 0.52, 0.52, 78);

	mat4 translateAll = Translate(-1.25, 0.45, 0);
	instance = translateAll * Translate(0, 0, 0) * Scale(1.5, 0.2, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = translateAll * Translate(-1.05, 0, 0) * Scale(0.6, 0.2, 0.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = translateAll * Translate(-1.4, 0, 0) * Scale(0.1, 0.2, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = translateAll * Translate(-1.75, 0, 0) * Scale(0.6, 0.2, 0.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	
	instance = translateAll * Translate(-2.15, 0, 0) * Scale(0.2, 0.2, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = translateAll * Translate(-1.05, 0.09, 0.4) * Scale(0.6, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = translateAll * Translate(-1.05, 0.09, -0.4) * Scale(0.6, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = translateAll * Translate(-1.75, 0.09, 0.4) * Scale(0.6, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = translateAll * Translate(-1.75, 0.09, -0.4) * Scale(0.6, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_Xe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void ThanXe(mat4 model_Xe) {
	DauXe(model_Xe);
	DuoiXe(model_Xe);
}

// Tốc độ xe tải
GLfloat X_XeTai = 0,
		Z_XeTai = 0,
		RY_XeTai = 0;
double	speedFW_XeTai = 0.0f, // Tốc độ tiến
		speedFW_R_XeTai = 0.0f, // Tốc độ tiến phải
		speedFW_L_XeTai = 0.0f,	// Tốc độ tiến trái
		speedBK_XeTai = 0.0f,  // Tốc độ lùi
		speedBK_L_XeTai = 0.0f,	// Tốc độ lùi trái
		speedBK_R_XeTai = 0.0f;	// Tốc độ lùi phải
bool	isFW = false, // Tiến ?
		isFW_R = false, // Tiến phải ?
		isFW_L = false, // Tiến trái ?
		isBK = false, // Lùi ?
		isBK_R = false, // Lùi phải ?
		isBK_L = false; // Lùi Trái ?


GLfloat RY_BanhXe = 0;
vec4 ToaDoXe = vec4(0, 0, 0, 1);
vec4 transformed_Xe;
void XeTai(float x, float y, float z, bool status) {
	mat4 model_Xe = model * Translate(x,y,z);

	mat4 model_BanhXe_Rotate = RotateZ(0);

	if (status) {
		model_Xe *= Translate(X_XeTai, 0, Z_XeTai) * RotateY(RY_XeTai) * Translate(2.65, 0, 0);
		model_BanhXe_Rotate = RotateZ(RY_BanhXe);

		tranformed = model_Xe * Translate(-2,0,0) * vec4(0, 0, 0, 1);
		ToaDo_XeTai[0] = tranformed.x / tranformed.w;
		ToaDo_XeTai[1] = tranformed.y / tranformed.w;
		ToaDo_XeTai[2] = tranformed.z / tranformed.w;
	}

	ThanXe(model_Xe);


	mat4 model_banhxe1 = model_Xe * Translate(-0.2, 0.25, 0.4) * RotateZ(RY_BanhXe);
	mat4 model_banhxe2 = model_Xe * Translate(-0.2, 0.25, -0.4) * RotateZ(RY_BanhXe);
	mat4 model_banhxe3 = model_Xe * Translate(-2.3, 0.25, 0.4) * RotateZ(RY_BanhXe);
	mat4 model_banhxe4 = model_Xe * Translate(-2.3, 0.25, -0.4) * RotateZ(RY_BanhXe);
	mat4 model_banhxe5 = model_Xe * Translate(-3.0, 0.25, 0.4) * RotateZ(RY_BanhXe);
	mat4 model_banhxe6 = model_Xe * Translate(-3.0, 0.25, -0.4) * RotateZ(RY_BanhXe);

	BanhXe(model_banhxe1);
	BanhXe(model_banhxe2);
	BanhXe(model_banhxe3);
	BanhXe(model_banhxe4);
	BanhXe(model_banhxe5);
	BanhXe(model_banhxe6);
}

// Địa hình bến cảng

void MatSan() { // Mặt sân
	ChangeColor(0.25, 0.25, 0.25, 0.4, 0.4, 0.4, 0.52, 0.52, 0.52, 78);
	instance = Translate(0, -1.5, -48) * Scale(300, 3, 100);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void VachKeDuong() {
	ChangeColor(0.86, 0.7, 0, 0.81, 0.7, 0, 0.53, 0.53, 0.53, 78);

	instance = Translate(0, 0.01, 1.0) * Scale(300, 0.02, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 0.01, -15.0) * Scale(300, 0.02, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	for (int i = 0; i < 300; i++) {
		instance = Translate(1.0 * i - 150, 0.01, -3) * Scale(0.5, 0.02, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	for (int i = 0; i < 300; i++) {
		instance = Translate(1.0 * i - 150, 0.01, -7) * Scale(0.5, 0.02, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	for (int i = 0; i < 300; i++) {
		instance = Translate(1.0 * i - 150, 0.01, -11) * Scale(0.5, 0.02, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	instance = Translate(30, 0.01, -56.5) * Scale(0.1, 0.02, 83);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(38, 0.01, -56.5) * Scale(0.1, 0.02, 83);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	for (int i = 0; i < 83; i++) {
		instance = Translate(34, 0.01, -15 - i - 0.25) * Scale(0.1, 0.02, 0.5);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
 }

void HangDa() {  // Hàng đá

	ChangeColor(0, 0, 0, 0.35, 0.35, 0.35, 0.53, 0.53, 0.53, 78);
	for (int i = 0; i < 50; i++) {
		instance = RotateX(90) * RotateZ(90) * Translate(1.5, i * 3, 0) * Scale(0.5, 0.5, 0.5);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
		glDrawArrays(GL_TRIANGLES, NumPoints, NumPoints2);
	}

	for (int i = 0; i < 50; i++) {
		instance = RotateX(90) * RotateZ(90) * Translate(1.5, -i * 3, 0) * Scale(0.5, 0.5, 0.5);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
		glDrawArrays(GL_TRIANGLES, NumPoints, NumPoints2);
	}
}

void SanCang() {
	MatSan();
	VachKeDuong();
	HangDa();
}

void MatNuoc() {
	ChangeColor(0, 0, 0.8, 0, 0.8, 1, 0.52, 0.52, 0.52, 78);
	instance = Translate(0, -4.5, 27) * Scale(300, 5, 50);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void DiaHinh() {
	SanCang();
	MatNuoc();
}
// Thung Hang
void ThungHang(int status, mat4 model_control) { // Trạng thái 1: Hàng trên xe tải, 2: Hàng Trên Tàu, 3: Hàng được móc bởi cần cẩu
	ChangeColor(0.2, 0.2, 0.3, 0.1, 0.2, 0.6, 0.52, 0.52, 0.52, 78);
	if (is_Moc) {
		model_control = model_Moc;
		model_ThungHang = model_Moc;
	}
	if (!is_Moc && is_Moved) {
		model_control = model_ThungHang;
	}
	if (status == 3) {
		tranformed = model_control * vec4(0, 0, 0, 1);
		ToaDo_ThungHang[0] = tranformed.x / tranformed.w;
		ToaDo_ThungHang[1] = tranformed.y / tranformed.w;
		ToaDo_ThungHang[2] = tranformed.z / tranformed.w;
	}
	mat4 TranslateAll = Translate(0, 0, 0);;
	if (status == 1)
		TranslateAll = Translate(X_XeTai, 0, Z_XeTai) * RotateY(RY_XeTai) * Translate(0.66, 1.05, 0);

	instance = TranslateAll * Translate(0, 0, 0) * Scale(2.8,0.98,0.98);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	ChangeColor(0.2, 0.2, 0.2, 0.8, 0.2, 0, 0.52, 0.52, 0.52, 78);
	// thanh doc
	instance = TranslateAll * Translate(1.4, 0, 0.48) * Scale(0.04, 1, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = TranslateAll * Translate(-1.4, 0, -0.48) * Scale(0.04, 1, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = TranslateAll * Translate(1.4, 0, -0.48) * Scale(0.04, 1, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = TranslateAll * Translate(-1.4, 0, 0.48) * Scale(0.04, 1, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//thang ngang
	instance = TranslateAll * Translate(0, 0.48, 0.48) * Scale(2.8, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = TranslateAll * Translate(0, -0.48, -0.48) * Scale(2.8, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = TranslateAll * Translate(0, 0.48, -0.48) * Scale(2.8, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = TranslateAll * Translate(0, -0.48, 0.48) * Scale(2.8, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// Thanh ngang
	instance = TranslateAll * Translate(1.4, 0.48, 0) * Scale(0.04, 0.04, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = TranslateAll * Translate(1.4, -0.48, 0) * Scale(0.04, 0.04, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = TranslateAll * Translate(-1.4, 0.48, 0) * Scale(0.04, 0.04, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = TranslateAll * Translate(-1.4, -0.48, 0) * Scale(0.04, 0.04, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// Chốt cửa
	ChangeColor(0.4, 0.4, 0.4, 0.2, 0.2, 0.2, 0.52, 0.52, 0.52, 78);
	instance = TranslateAll * Translate(-1.41, 0, -0.06) * Scale( 0.02, 0.92, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));

	instance = TranslateAll * Translate(-1.41, 0, 0.06) * Scale(0.02, 0.92, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));

	instance = TranslateAll * Translate(-1.40, 0, 0) * Scale(0.02, 0.92, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	ChangeColor(0.2, 0.2, 0.4, 0.1, 0.2, 0.6, 0.52, 0.52, 0.52, 78);
	for (int i = 0; i < 12; i++) {
		instance = TranslateAll * Translate(-1.4 + 0.03 + (i * 0.24), 0, 0.484) * Scale(0.08, 0.98, 0.02);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = TranslateAll * Translate(-1.4 + 0.03 + (i * 0.24), 0, -0.484) * Scale(0.08, 0.98, 0.02);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

void ThungHangTrenBen(float x, float y, float z, int hang, int cot, int tang) {
	mat4 model_ThungHangTrenBen = model * Translate(-3.1*(1.0*cot/2), 0, -1.1*(1.0*hang/2)) * Translate(x, y, z);
	for (int i = 0; i < tang; i++) {
		for (int j = 0; j < hang; j++) {
			for (int k = 0; k < cot; k++) {
				model_ThungHangTrenBen *= Translate(3.1, 0, 0);
				ThungHang(0, model_ThungHangTrenBen);
			}
			model_ThungHangTrenBen *= Translate(-cot * 3.1, 0, 1.1);
		}
		model_ThungHangTrenBen *= Translate(0, 1.02, -1.1* hang);
	}
}

void DayTau(mat4 model_control) {
	ChangeColor(0.0, 0.0, 0.8, 0.0, 0.24, 1, 0.52, 0.52, 0.52, 78);
	mat4 TranslateAll = model_control;
	instance = RotateX(-90) * RotateZ(90) * Translate(0, 0, 1) * Scale(8, 16, 8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, NumPoints, NumPoints2);
	
	instance = RotateX(90) * RotateZ(-90) * Translate(0, 8, 3) * Scale(8, 12, 8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, NumPoints + NumPoints2, NumPoints3);

	instance = RotateX(90) * RotateZ(90) * Translate(0, 8, 3) * Scale(8, 8, 8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, NumPoints + NumPoints2, NumPoints3);

}

void BoongTau(mat4 model_control) {
	mat4 TranslateAll = model_control * Translate(0,1.4,0);
	// Tầng 1
	ChangeColor(0.1, 0.1, 0.1, 0.6, 0.6, 0.6, 0.51, 0.52, 0.52, 78);
	instance = Translate(-7, 0.75, 0) * Scale(3, 1.5, 6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	// Mái tầng 1
	ChangeColor(0.1, 0.1, 0.1, 0.4, 0.4, 0.4, 0.52, 0.52, 0.52, 78);
	instance = Translate(-7, 1.52, 0) * Scale(3.2, 0.04, 6.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	// Tầng 2
	ChangeColor(0.1, 0.1, 0.1, 0.6, 0.6, 0.6, 0.51, 0.52, 0.52, 78);
	instance = Translate(-7, 2.02, 0) * Scale(1.8, 1, 4.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	// Mái tầng 2
	ChangeColor(0.1, 0.1, 0.1, 0.4, 0.4, 0.4, 0.52, 0.52, 0.52, 78);
	instance = Translate(-7, 2.56, 0) * Scale(2.0, 0.04, 4.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	ChangeColor(0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 0.52, 0.52, 0.52, 78);
	// Kính trên tàu
	for (int i = 0; i < 10; i++) {
		instance = Translate(-5.5, 1.0, -2.7 + (i*0.6)) * Scale(0.02, 0.4, 0.4);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(-8.5, 1.0, -2.7 + (i * 0.6)) * Scale(0.02, 0.4, 0.4);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	for (int i = 0; i < 5; i++) {
		instance = Translate(-8.2 + (i*0.6), 1.0, 3) * Scale(0.4, 0.4, 0.02);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(-8.2 + (i * 0.6), 1.0, -3) * Scale(0.4, 0.4, 0.02);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	for (int i = 0; i < 7; i++) {
		instance = Translate(-6.1, 2, -1.8 + (i * 0.6)) * Scale(0.02, 0.4, 0.4);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(-7.9, 2, -1.8 + (i * 0.6)) * Scale(0.02, 0.4, 0.4);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	for (int i = 0; i < 3; i++) {
		instance = Translate(-7.6 + (i*0.6), 2, 2.1) * Scale(0.4, 0.4, 0.02);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	}
}

void HangTrenTau(mat4 model_control) {
	mat4 TranslateAll = model_control * Translate(0,1.9,0);
	TranslateAll *= Translate(-3,0,-3);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 7; j++) {
			for (int k = 0; k < 4; k++) {
				ThungHang(0, TranslateAll);
				TranslateAll *= Translate(2.8, 0, 0);
			}
			TranslateAll *= Translate(-2.8*4, 0, 1);
		}
		TranslateAll *= Translate(0, 1, -1*7);
	}
}

void SanTau(mat4 model_control) {
	mat4 TranslateAll = model_control;

	ChangeColor(0.3, 0, 0, 0.8, 0.2, 0, 0.52, 0.52, 0.52, 76);
	instance = Translate(0, 1.2, 0) * Scale(16, 0.4, 8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = RotateY(90) * Translate(0, 1.2, -8) * Scale(8, 0.4, 8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, NumPoints, NumPoints2);

	instance = RotateY(-90) * Translate(0, 1.2, -8) * Scale(8, 0.4, 12);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, NumPoints, NumPoints2);


	// Lan can
	ChangeColor(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.51, 0.52, 0.52, 76);

	instance = Translate(0, 1.7, 3.98) * Scale(16, 0.6, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 1.7, -3.98) * Scale(16, 0.6, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(11, 1.9, 2) * RotateY(33.69) * Scale(7.21, 1, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(11, 1.9, -2) * RotateY(-33.69) * Scale(7.21, 1, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-10, 1.7, -2) * RotateY(45) * Scale(5.65, 0.6, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-10, 1.7, 2) * RotateY(-45) * Scale(5.65, 0.6, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, TranslateAll * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	BoongTau(model_control);
	HangTrenTau(TranslateAll);
}

// Tốc độ tàu
GLfloat X_Tau = 0,
		Z_Tau = 0,
		RY_Tau = 0;
double	speedFW_Tau = 0.0f, // Tốc độ tiến
		speedFW_R_Tau = 0.0f, // Tốc độ tiến phải
		speedFW_L_Tau = 0.0f,	// Tốc độ tiến trái
		speedBK_Tau = 0.0f,  // Tốc độ lùi
		speedBK_L_Tau = 0.0f,	// Tốc độ lùi trái
		speedBK_R_Tau = 0.0f;	// Tốc độ lùi phải
bool	isFW_Tau = false, // Tiến ?
		isFW_R_Tau = false, // Tiến phải ?
		isFW_L_Tau = false, // Tiến trái ?
		isBK_Tau = false, // Lùi ?
		isBK_R_Tau = false, // Lùi phải ?
		isBK_L_Tau = false; // Lùi Trái ?

void Tau() {
	mat4 model_Tau = model;
	model_Tau *= Translate(X_Tau, 0, Z_Tau + 16) * RotateY(RY_Tau);
	DayTau(model_Tau);
	SanTau(model_Tau);
}

void BeNhienLieu(mat4 model_control) {
	instance = Translate(0, 3.5, 0) * Scale(4, 5, 4);
	ChangeColor(0.2, 0.2, 0.2, 0.6, 0.6, 0.6, 0.52, 0.52, 0.52, 76);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));

	ChangeColor(0, 0, 0.3, 0, 0.2, 1, 0.51, 0.51, 0.51, 76);
	instance = Translate(0, 6.05, 0) * Scale(4, 0.2, 4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));

	ChangeColor(0.2, 0.2, 0.2, 0, 0, 0, 0.52, 0.52, 0.52, 76);
	float x,y;
	float alpha = 0;
	for (int i = 0; i < 8; i++) {
		x = cos(DegreesToRadians * alpha) * 1.9;
		y = sin(DegreesToRadians * alpha) * 1.9;
		instance = Translate(x, 0.5, y) * Scale(0.2,1,0.2);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		alpha += 45;
	}
}

void TruXang(float x, float y, float z, mat4 model_control) {
	ChangeColor(0, 0, 0.4, 0, 0.2, 1, 0.52, 0.52, 0.52, 78);
	instance = Translate(x, y, z) * Scale(0.4, 1, 0.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	ChangeColor(0.78, 0, 0, 0.6, 0.2, 0, 0.5, 0.5, 0.5, 76);
	instance = Translate(x, y + 0.7, z - 0.35) * Scale(0.4, 0.4, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(x, y + 0.7, z + 0.35) * Scale(0.4, 0.4, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(x, y + 0.9, z) * Scale(0.4, 0.1, 0.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void CayXang(mat4 model_control) {
	model_control *= Translate(-18, 0, -3);
	ChangeColor(0, 0, 0.4, 0, 0.21, 1, 0.5, 0.5, 0.5, 76);
	// Cột
	instance = Translate(0, 1.5, 3.0) * Scale(0.4, 3, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 1.5, -3.0) * Scale(0.4, 3, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(3.0, 1.5, 3.0) * Scale(0.4, 3, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(3.0, 1.5, -3.0) * Scale(0.4, 3, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// Mái
	ChangeColor(0.78, 0, 0, 0.6, 0.2, 0, 0.5, 0.5, 0.5, 76);
	instance = Translate(0, 3.03, 0) * Scale(6.4, 0.06, 10);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	ChangeColor(0, 0, 0.4, 0, 0.21, 1, 0.5, 0.5, 0.5, 76);
	instance = Translate(0, 3.16, 0) * Scale(6.4, 0.2, 10);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// Trụ xăng

	TruXang(0, 0.5, -2.0, model_control);
	TruXang(0, 0.5, 2.0, model_control);
	TruXang(3, 0.5, -2.0, model_control);
	TruXang(3, 0.5, 2.0, model_control);
}

void KhoNhienLieu() {
	mat4 model_control = model * Translate(50, 0, -40);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			BeNhienLieu(model_control);
			model_control *= Translate(0, 0, -5);
		}
		model_control *= Translate(5, 0, 5 * 4);
	}

	CayXang(model_control);
}

void CabinCanCau(mat4 model_control) {
	// Cabin
	ChangeColor(0.3, 0, 0, 0.8, 0.2, 0, 0.52, 0.52, 0.52, 76);
	instance = Translate(0, 0, -0.5) * Scale(1.6, 1, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, -0.25,0.25) * Scale(1.6, 0.5, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = RotateZ(90) * RotateY(45) * Translate(0, 0, 0.35) * Scale(0.707, 1.6, 0.707);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, NumPoints, NumPoints2);

	// Kính
	ChangeColor(0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 0.52, 0.52, 0.52, 78);
	instance = Translate(0, 0.25, 0.25) * RotateX(45) * Scale(1.5, 0.02, 0.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0.8, 0.25, -0.5) * Scale(0.02, 0.4, 0.9);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-0.8, 0.25, -0.5) * Scale(0.02, 0.4, 0.9);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void BonChuaXang(mat4 model_control) {
	instance = Translate(0, 0, 0) * RotateX(90) * RotateZ(90) * Scale(1.4, 2.8, 1);
	ChangeColor(1, 1, 1, 0.8, 0.8, 0.8, 0.52, 0.52, 0.52, 78);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));

	ChangeColor(0.7, 0.7, 0.7, 0.5, 0.5, 0.5, 0.52, 0.52, 0.52, 78);
	instance = Translate(0, 0.5, 0) * Scale(2.6, 0.2, 0.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	ChangeColor(0.1, 0.1, 0.1, 0.4, 0.4, 0.4, 0.52, 0.52, 0.52, 78);
	for (int i = 0; i < 4; i++) {
		instance = Translate(-0.9 + i * 0.6, 0.60, 0) * Scale(0.4, 0.1, 0.4);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
		glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
		glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));
	}

	ChangeColor(0, 0, 0.4, 0, 0.2, 1, 0.52, 0.52, 0.52, 78);
	instance = Translate(0, -0.4, 0.58) * RotateX(90) * RotateZ(90) * Scale(0.2, 2.8, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));

	instance = Translate(0, -0.4, -0.58) * RotateX(90) * RotateZ(90) * Scale(0.2, 2.8, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));
}

void DayCapCanCau(mat4 model_control) {
	instance = Scale(0.1, 1, 0.1);

	ChangeColor(0, 0, 0, 0.3, 0.3, 0.3, 0.52, 0.52, 0.52, 78);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MocCanCau(mat4 model_control) {
	ChangeColor(0.8, 0.6, 0.02, 0.5, 0.4, 0, 0.52, 0.52, 0.52, 78);

	instance = Scale(0.1, 0.1, 0.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 0, -0.45) * Scale(3, 0.1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 0, 0.45) * Scale(3, 0.1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void TangDa(mat4 model_control) {
	ChangeColor(0.4, 0.4, 0.4, 0.6, 0.6, 0.6, 0.52, 0.52, 0.52, 78);
	instance = Scale(2, 3, 2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void ThanTrenCanCau(mat4 model_control) {
	instance = Scale(2.2, 0.4, 2.2);
	ChangeColor(0.1, 0.1, 0.1, 0.2, 0.2, 0.2, 0.52, 0.52, 0.52, 78);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));

	ChangeColor(0, 0, 0, 0.3, 0.3, 0.3, 0.52, 0.52, 0.52, 78);
	instance = Translate(0, 0.05, 5) * Scale(0.1, 0.1, 30);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	ChangeColor(0.8, 0.6, 0.02, 0.5, 0.4, 0, 0.52, 0.52, 0.52, 78);
	instance = Translate(0.5, 0.05, 5) * Scale(0.1, 0.1, 30);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-0.5, 0.05, 5) * Scale(0.1, 0.1, 30);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0.5, 1.05, 5) * Scale(0.1, 0.1, 30);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-0.5, 1.05, 5) * Scale(0.1, 0.1, 30);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	for (int i = 0; i < 31; i++) {
		instance = Translate(-0.5, 0.6, -10 + i) * Scale(0.1, 1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0.5, 0.6, -10 + i) * Scale(0.1, 1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0, 1.05, -10 + i) * Scale(1, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0, 0.05, -10 + i) * Scale(1, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		if (i < 30) {
			instance = Translate(0, 1.05, -10 + i + 0.5) * RotateY(45) * Scale(1.41, 0.1, 0.1);
			glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
			glDrawArrays(GL_TRIANGLES, 0, NumPoints);

			instance = Translate(0, 0.05, -10 + i + 0.5) * RotateY(-45) * Scale(1.41, 0.1, 0.1);
			glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
			glDrawArrays(GL_TRIANGLES, 0, NumPoints);

			instance = Translate(-0.5, 0.6, -10 + i + 0.5) * RotateX(45) * Scale(0.1, 1.41, 0.1);
			glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
			glDrawArrays(GL_TRIANGLES, 0, NumPoints);

			instance = Translate(0.5, 0.6, -10 + i + 0.5) * RotateX(45) * Scale(0.1, 1.41, 0.1);
			glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
			glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		}
	}

	instance = Translate(-0.5, 2, -0.5) * Scale(0.1, 2, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0.5, 2, -0.5) * Scale(0.1, 2, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-0.5, 2, 0.5) * Scale(0.1, 2, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0.5, 2, 0.5) * Scale(0.1, 2, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	for (int i = 0; i < 2; i++) {
		instance = Translate(-0.5, 1.5 + i, 0) * RotateX(45) * Scale(0.1, 0.1, 1.41);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(-0.5, 1.5 + i, 0) * RotateX(-45) * Scale(0.1, 0.1, 1.41);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0.5, 1.5 + i, 0) * RotateX(45) * Scale(0.1, 0.1, 1.41);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0.5, 1.5 + i, 0) * RotateX(-45) * Scale(0.1, 0.1, 1.41);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0, 1.5 + i, -0.5) * RotateZ(45) * Scale(1.41, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0, 1.5 + i, -0.5) * RotateZ(-45) * Scale(1.41, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0, 1.5 + i, 0.5) * RotateZ(45) * Scale(1.41, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0, 1.5 + i, 0.5) * RotateZ(-45) * Scale(1.41, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(-0.5, 2 + i, 0) * Scale(0.1, 0.1, 1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0.5, 2 + i, 0) * Scale(0.1, 0.1, 1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0, 2 + i, -0.5) * Scale(1, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0, 2 + i, 0.5) * Scale(1, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	// Dây cáp cân bằng
	ChangeColor(0, 0, 0, 0.3, 0.3, 0.3, 0.52, 0.52, 0.52, 78);
	instance = Translate(-0.5, 2.05, -5.2) * RotateX(-11.5) * Scale(0.1, 0.1, 9.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control* instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0.5, 2.05, -5.2) * RotateX(-11.5) * Scale(0.1, 0.1, 9.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control* instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0.5, 2.0, 10.2) * RotateX(5.9) * Scale(0.1, 0.1, 19.68);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control* instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-0.5, 2.0, 10.2) * RotateX(5.9) * Scale(0.1, 0.1, 19.68);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control* instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// Cabin
	CabinCanCau(model_control* Translate(1.4, 1, 0.25));
	// Tảng đá cân bằng

	TangDa(model_control* Translate(0, 0, -11.5));
}

void ThanDuoiCanCau(mat4 model_control) {
	instance = Translate(0,0.2, 0) * Scale(3.2, 0.4, 3.2);

	ChangeColor(0.2, 0.2, 0.2, 0.4, 0.4, 0.4, 0.52, 0.52, 0.52, 78);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_FAN, NumPoints + NumPoints2 + NumPoints3 + NumPoints4 + 2, NumPoints4 + 2);
	glDrawArrays(GL_TRIANGLE_STRIP, NumPoints + NumPoints2 + NumPoints3 + 2 * (NumPoints4 + 2), 2 * (NumPoints4 + 1));

	ChangeColor(0.8, 0.6, 0.02, 0.5, 0.4, 0, 0.52, 0.52, 0.52, 78);
	instance = Translate(-1, 5.4, -1) * Scale(0.2, 10, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(1, 5.4, -1) * Scale(0.2, 10, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-1, 5.4, 1) * Scale(0.2, 10, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(1, 5.4, 1) * Scale(0.2, 10, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	for (int i = 0; i < 5; i++) {
		// X trong
		instance = Translate(0, 0.4+1+i*2, -1) * RotateZ(45) * Scale(2.83, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0, 0.4 + 1 + i * 2, -1) * RotateZ(-45) * Scale(2.83, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		// X ngoài
		instance = Translate(0, 0.4 + 1 + i * 2, 1) * RotateZ(45) * Scale(2.83, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(0, 0.4 + 1 + i * 2, 1) * RotateZ(-45) * Scale(2.83, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		// X trái
		instance = Translate(-1, 0.4 + 1 + i * 2, 0) * RotateX(45) * Scale(0.1, 0.1, 2.83);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(-1, 0.4 + 1 + i * 2, 0) * RotateX(-45) * Scale(0.1, 0.1, 2.83);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		// X phải
		instance = Translate(1, 0.4 + 1 + i * 2, 0) * RotateX(45) * Scale(0.1, 0.1, 2.83);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		instance = Translate(1, 0.4 + 1 + i * 2, 0) * RotateX(-45) * Scale(0.1, 0.1, 2.83);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	for (int i = 0; i < 4; i++) {
		// Thanh ngang sau
		instance = Translate(0, 2.4 + 2 * i, -1) * Scale(2, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		// Thanh ngang trước
		instance = Translate(0, 2.4 + 2 * i, 1) * Scale(2, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		// Thanh ngang trai
		instance = Translate(1, 2.4 + 2 * i, 0) * Scale(0.1, 0.1, 2);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		// Thanh ngang phải
		instance = Translate(-1, 2.4 + 2 * i, 0) * Scale(0.1, 0.1, 2);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	// Giá đỡ
	ChangeColor(0.2, 0.2, 0.2, 0.4, 0.4, 0.4, 0.52, 0.52, 0.52, 78);
	instance = Translate(0,10.5, 0) * Scale(2.6, 0.2, 2.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

float Y_Day = 0.5, RY_DAY = 0, Z_Day = 0, XoayZ = 0, XoayZ_TAM = 0, XoayX = 0, XoayX_TAM = 0;
bool is_Xoay_Z = false, is_Xoay_X = false;


void CanCau(float x, float y, float z) {
	mat4 model_control = model * Translate(x,y,z);
	ThanDuoiCanCau(model_control);

		model_control *= RotateY(RY_DAY) * Translate(0, 10.8, 0);
		ThanTrenCanCau(model_control);

		model_control *= Translate(0, 0, 20 - Z_Day) * RotateX(XoayX) * RotateZ(XoayZ) * Translate(0, -(Y_Day / 2), 0) * Scale(1, Y_Day, 1);
		DayCapCanCau(model_control);

		model_control *= Scale(1, 1 / Y_Day, 1) *  Translate(0, -(Y_Day / 2), 0);
		MocCanCau(model_control);

		model_Moc = model_control * Translate(0, -0.55, 0);

		tranformed = model_Moc * vec4(0, 0, 0, 1);
		ToaDo_Moc[0] = tranformed.x / tranformed.w;
		ToaDo_Moc[1] = tranformed.y / tranformed.w;
		ToaDo_Moc[2] = tranformed.z / tranformed.w;
}

void XeBon(float x, float y, float z) {
	XeTai(x, y, z, false);
	mat4 model_XeBon = model * Translate(x, y, z);
	model_XeBon *= Translate(-2, 1.06, 0);
	BonChuaXang(model_XeBon);
}

// Nhà kho

mat4 model_kho, model_cua1, model_cua2, model_quat, model_kho1;
// Äá»‹a hÃ¬nh báº¿n cáº£ng
// Thiáº¿t láº­p mÃ u sáº¯c cho cÃ¡c thÃ nh pháº§n cá»§a nhÃ  kho
void setMauNhaKho() {
	material_ambient = color4(0.61f, 0.50f, 0.42f, 1.0f);
	material_diffuse = color4(0.45f, 0.34f, 0.29f, 1.0f);
	material_specular = color4(0.31f, 0.28f, 0.27f, 1.0f);
	material_shininess = 51.0f;
	SetUpColor();
}
void setMauMaiNhakho() {
	material_ambient = color4(0.8f, 1.0f, 0.4f, 1.0f); // mÃ u xanh nÃµn chuá»‘i
	material_diffuse = color4(0.8f, 1.0f, 0.4f, 1.0f); // mÃ u xanh nÃµn chuá»‘i
	material_specular = color4(0.8f, 1.0f, 0.4f, 1.0f); // mÃ u xanh nÃµn chuá»‘i
	material_shininess = 51.0f;
	SetUpColor();
}
void setMauTuongNhakho() {
	material_ambient = color4(0.5f, 0.5f, 0.5f, 1.0f); // mÃ u xÃ¡m
	material_diffuse = color4(0.6f, 0.6f, 0.6f, 1.0f); // mÃ u xÃ¡m
	material_specular = color4(0.7f, 0.7f, 0.7f, 1.0f); // mÃ u xÃ¡m
	material_shininess = 51.0f;
	SetUpColor();
}
void setMauDieuHoa() {
	material_ambient = color4(0.9f, 0.9f, 0.9f, 1.0f); // mÃ u tráº¯ng sÃ¡ng
	material_diffuse = color4(1.0f, 1.0f, 1.0f, 1.0f); // mÃ u tráº¯ng sÃ¡ng
	material_specular = color4(1.0f, 1.0f, 1.0f, 1.0f); // mÃ u tráº¯ng sÃ¡ng
	material_shininess = 76.8f;
	SetUpColor();
}
void setMauQuatGio() {
	material_ambient = color4(0.0f, 0.0f, 0.0f, 1.0f); // mÃ u Ä‘en
	material_diffuse = color4(0.0f, 0.0f, 0.0f, 1.0f); // mÃ u Ä‘en
	material_specular = color4(0.0f, 0.0f, 0.0f, 1.0f); // mÃ u Ä‘en
	material_shininess = 76.8f;
	SetUpColor();
}

// Váº½ cÆ¡ sá»Ÿ nhÃ  kho
void Tuong1(GLfloat x, GLfloat y, GLfloat z) {
	setMauTuongNhakho();
	mat4 instance = Translate(x, y, z) * Scale(8.2, 6, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho * model_kho1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void Tuong2(GLfloat x, GLfloat y, GLfloat z) {
	setMauTuongNhakho();
	mat4 instance = Translate(x, y, z) * Scale(0.2, 6, 8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho * model_kho1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void Mai(GLfloat x, GLfloat y, GLfloat z) {
	setMauTuongNhakho();
	mat4 instance = RotateX(90) * RotateZ(90) * Translate(x, y, z) * Scale(8, 0.2, 3);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho * model_kho1 * instance);
	glDrawArrays(GL_TRIANGLES, NumPoints, NumPoints2);
}
void MaiChe1(GLfloat x, GLfloat y, GLfloat z) {
	setMauMaiNhakho();
	mat4 instance = RotateX(20) * Translate(x, y, z) * Scale(8.2, 0.2, 4.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho * model_kho1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void MaiChe2(GLfloat x, GLfloat y, GLfloat z) {
	setMauMaiNhakho();
	mat4 instance = RotateX(-20) * Translate(x, y, z) * Scale(8.2, 0.2, 4.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho * model_kho1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void ThanhCua1(GLfloat x, GLfloat y, GLfloat z) {
	setMauTuongNhakho();
	mat4 instance = Translate(x, y, z) * Scale(8.2, 1.5, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho * model_kho1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void ThanhCua2(GLfloat x, GLfloat y, GLfloat z) {
	setMauTuongNhakho();
	mat4 instance = Translate(x, y, z) * Scale(2, 4.5, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho * model_kho1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void canhdieuhoa(GLfloat x, GLfloat y, GLfloat z) {
	mat4 instance = Translate(x, y, z) * Scale(2, 4.5, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho * model_kho1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void Thandieuhoa() {
	setMauDieuHoa();
	mat4 instance = Scale(0.4, 1, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho * model_kho1 * model_quat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void canhQuat(GLfloat r) {
	// Set the color for the fan
	setMauQuatGio();

	// Create the instance matrix
	mat4 instance = RotateX(90) * RotateY(r) * Translate(0.5, 0, 0) * Scale(1, 0.2, 0.6);

	// Pass the instance matrix to the shader
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho * model_kho1 * model_quat * instance);

	// Draw the triangles for the fan
	glDrawArrays(GL_TRIANGLES, NumPoints, NumPoints2);  // Assuming NumPoints2 is the number of points for the fan
}
void quatGio() {
	canhQuat(30);
	canhQuat(-90);
	canhQuat(150);


}
void cua1() {
	setMauDieuHoa();
	/**Scale(2.9, 4.5, 0.2);*/
	/*mat4 instance = Translate(2.45, -0.75, -0.1);*/
	mat4 instance = Scale(2.9, 4.5, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho1 * model_cua1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void cua2() {
	setMauDieuHoa();
	/**Scale(2.9, 4.5, 0.2);*/
	/*mat4 instance = Translate(2.45, -0.75, -0.1);*/
	mat4 instance = Scale(2.9, 4.5, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_kho1 * model_cua2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
// Váº½ toÃ n bá»™ nhÃ  kho báº±ng cÃ¡ch káº¿t há»£p cÆ¡ sá»Ÿ vÃ  mÃ¡i
GLfloat y_cua1 = 1, z_cua, y_cua2 = 1, f_quat = 0;
void dieuhoa(GLfloat x, GLfloat y, GLfloat z) {
	model_quat = Translate(x, y, z) * Translate(4.3, 1, -1);
	Thandieuhoa();
	model_quat = model_quat * Translate(0.3, 0, 0) * RotateY(90) * RotateZ(f_quat) * Scale(0.5, 0.5, 0.5);
	quatGio();
}


void Nhakho(float x, float y, float z) {
	model_kho1 = Translate(x, y, z) * Translate(0, 5.2, 0);
	model_kho = Translate(x, y, z) * Translate(0, -2.25, 0);
	Tuong1(0, 0, -8);
	Tuong2(4, 0, -4);
	Tuong2(-4, 0, -4);
	Mai(-4, 4, -3);
	Mai(-4, -4, -3);
	MaiChe1(0, 2.9, -3);
	MaiChe2(0, 5.6, -4.3);
	ThanhCua1(0, 2.25, -0.1);
	ThanhCua2(0, -0.75, -0.1);
	dieuhoa(0, 0, 0);
	dieuhoa(0, 0, -2.5);
	dieuhoa(0, 0, -5);
	model_cua1 = Translate(x, y, z) * Scale(1, y_cua1, 1) * Translate(2.45, -3, -0.1);
	cua1();
	model_cua2 = Translate(x, y, z) * Scale(1, y_cua2, 1) * Translate(-2.45, -3, -0.1);
	cua2();
}

//   CẦN TRỤC


void setMauChanCan() {
	material_ambient = color4(0.31f, 0.1f, 0.84f, 1.0f);
	material_diffuse = color4(0.0f, 0.61424f, 0.26f, 1.0f);
	material_specular = color4(0.56f, 0.77f, 1.0f, 1.0f);
	material_shininess = 76.8f;
	SetUpColor();
}
void setMauThanCan() {
	material_ambient = color4(0.95f, 0.0f, 0.26f, 1.0f);
	material_diffuse = color4(0.74f, 0.57f, 0.13f, 1.0f);
	material_specular = color4(0.48f, 0.67f, 0.82f, 1.0f);
	material_shininess = 92.4f;
	SetUpColor();
}
void setMauDayMoc() {
	material_ambient = color4(0.97f, 0.64f, 0.35f, 1.0f);
	material_diffuse = color4(0.7f, 0.87f, 0.13f, 1.0f);
	material_specular = color4(0.48f, 0.67f, 0.82f, 1.0f);
	material_shininess = 65.8f;
	SetUpColor();
}
void setMauMoc() {
	material_ambient = color4(0.77f, 0.4f, 0.45f, 1.0f);
	material_diffuse = color4(0.79f, 0.21f, 0.11f, 1.0f);
	material_specular = color4(0.82f, 0.6f, 0.2f, 1.0f);
	material_shininess = 27.3f;
	SetUpColor();
}

void cotChanCan(GLfloat a, GLfloat b, GLfloat c, GLfloat chieuCao) {

	mat4 instance = Translate(a, b + (chieuCao / 2), c) * Scale(0.12, chieuCao, 0.12);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanhNgang(GLfloat a, GLfloat b, GLfloat c, GLfloat dai, GLfloat x, GLfloat y, GLfloat theta) {
	mat4 instance = Translate(a, b, c) * RotateX(theta) * Scale(x, y, dai);

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void trucDiChuyen(GLfloat dichuyen) {
	setMauThanCan();
	mat4 instance = Translate(0, 0, dichuyen) * Translate(0, 1.5, 1.7) * Scale(0.7, 0.1, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void dayMoc(GLfloat x, GLfloat dichuyen, GLfloat dodaiday) {
	mat4 instance = Translate(0, 0, dichuyen) * Translate(x, 1.5 - ((dodaiday + 0.4) / 2), 1.7) * Scale(0.01, dodaiday + 0.4, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	setMauDayMoc();
}
void moc(GLfloat dichuyen, GLfloat dodaiday) {

	mat4 instance = Translate(0, 0, dichuyen) * Translate(0, 1.5 - (dodaiday + 0.4), 1.7) * Scale(0.4, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	setMauMoc();
}


void chanCauThap() {

	cotChanCan(-0.4, 0, -0.4, 1.5);
	cotChanCan(0.4, 0, -0.4, 1.5);

}
void chanCauCao() {

	cotChanCan(0.4, 0, 0.4, 2);
	cotChanCan(-0.4, 0, 0.4, 2);
}
void trucCoDinh() {
	setMauThanCan();
	//thanh ngang duoi
	thanhNgang(0.4, 0.8, 0, 0.8, 0.05, 0.05, 0);
	thanhNgang(-0.4, 0.8, 0, 0.8, 0.05, 0.05, 0);

	//truc co dinh than
	thanhNgang(0.4, 1.1, 0, 0.9, 0.05, 0.05, -40);
	thanhNgang(-0.4, 1.1, 0, 0.9, 0.05, 0.05, -40);

	//truc chinh
	thanhNgang(0.4, 1.5, 0.5, 2.9, 0.18, 0.09, 0);
	thanhNgang(-0.4, 1.5, 0.5, 2.9, 0.18, 0.09, 0);

	//truc cá»‘ Ä‘á»‹nh 
	thanhNgang(0.4, 1.7, -0.13, 1.03, 0.05, 0.05, -25);
	thanhNgang(-0.4, 1.7, -0.13, 1.03, 0.05, 0.05, -25);

	//trá»¥c cá»‘ Ä‘á»‹nh 2
	thanhNgang(0.4, 1.7, 1.1, 1.5, 0.05, 0.05, 20);
	thanhNgang(-0.4, 1.7, 1.1, 1.5, 0.05, 0.05, 20);

	//trá»¥c co Ä‘á»‹nh 3
	thanhNgang(0, 1.5, 1.9, 0.1, 0.7, 0.09, 0);

	//cuc doi trong+ phÃ²ng Ä‘iá»u khiá»ƒn
	thanhNgang(0, 1.7, -0.7, 0.5, 0.9, 0.4, 0);
}

void chanCau() {
	setMauChanCan();
	chanCauThap();
	chanCauCao();
}
GLfloat dichuyen = 0, dodaiday = 0;
void dongDoc(GLfloat dichuyen, GLfloat dodaiday) {

	dayMoc(0.13, dichuyen, dodaiday);
	dayMoc(-0.13, dichuyen, dodaiday);
	moc(dichuyen, dodaiday);
	trucDiChuyen(dichuyen);
}


void BenCang() {
	DiaHinh();
	XeTai(0,0,0, true);
	ThungHang(3, model * Translate(0,0.5,-20));

	//ThungHangTrenBen(-19,0,-26, 5, 6, 3);
	
	Tau();

	CanCau(0, 0, -30);
	
	KhoNhienLieu();

	XeBon(46, 0, -32);
	XeBon(46, 0, -29);
	XeBon(46, 0, -26);
	Nhakho(10, 0, -9);

	model *= Translate(0,0,-3) * Scale(10, 10, 10);

	chanCau();
	trucCoDinh();
	dongDoc(dichuyen, dodaiday);
}

bool quatgioquay = false;
GLfloat xx = 1; // Chỉnh độ to nhỏ toàn bộ
GLfloat RX = 0, RY = 0, RZ = 0;
bool check1 = false, check2 = false;
void display( void )
{
	
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );      

	const vec3 viewer_pos(0.0, 0.0, 2.0);  /*Trùng với eye của camera*/

	if (quatgioquay) {
		if (f_quat > 360) f_quat -= 360;
			f_quat += 20; // TÄƒng gÃ³c quay lÃªn
	}

	// Dao động dây cần trục Z
	if (XoayZ > 5) {
		XoayZ = 5;
		XoayZ_TAM = -5;
	}

	if (XoayZ < -5) {
		XoayZ = -5;
		XoayZ_TAM = 5;
	}

	if (is_Xoay_Z) {
		if (abs(XoayZ_TAM) >= 0.01) {
			if (XoayZ_TAM < 0) {
				if (XoayZ > XoayZ_TAM - XoayZ_TAM / 6) {
					XoayZ += XoayZ_TAM / 6;
				}
				else {
					XoayZ_TAM -= XoayZ_TAM / 6;
					XoayZ_TAM *= -1;
				}
			}
			else {
				if (XoayZ < XoayZ_TAM - XoayZ_TAM / 6) {
					XoayZ += XoayZ_TAM / 6;
				}
				else {
					XoayZ_TAM -= XoayZ_TAM / 6;
					XoayZ_TAM *= -1;
				}
			}
		}
	}

	if (is_Xoay_X) {
		if (abs(XoayX_TAM) >= 0.01) {
			if (XoayX_TAM < 0) {
				if (XoayX > XoayX_TAM - XoayX_TAM / 6) {
					XoayX += XoayX_TAM / 6;
				}
				else {
					XoayX_TAM -= XoayX_TAM / 6;
					XoayX_TAM *= -1;
				}
			}
			else {
				if (XoayX < XoayX_TAM - XoayX_TAM / 6) {
					XoayX += XoayX_TAM / 6;
				}
				else {
					XoayX_TAM -= XoayX_TAM / 6;
					XoayX_TAM *= -1;
				}
			}
		}
	}

	cout << "Moc" << ToaDo_Moc[0] << " " << ToaDo_Moc[1] << " " << ToaDo_Moc[2];
	cout << "ThungHang" << ToaDo_ThungHang[0] << " " << ToaDo_ThungHang[1] << " " << ToaDo_ThungHang[2];
	cout << "XeTai" << ToaDo_XeTai[0] << " " << ToaDo_XeTai[1] << " " << ToaDo_XeTai[2] << endl;
	if (!is_Moc && is_Moved && ToaDo_ThungHang[1] > 0.6) {
		model_ThungHang *= Translate(0,-0.1,0);
	}

	model = RotateX(RX) * RotateY(RY) * RotateZ(RZ) * Scale(xx, xx, xx);
	BenCang();
	
	glutSwapBuffers();
}

float eyeX, eyeY, eyeZ;
void keyboard( unsigned char key, int x, int y )
{
	// keyboard handler

    switch ( key ) {
    case 033:			// 033 is Escape key octal value
        exit(1);		// quit program
        break;
	case 'x':
		RX += 5;
		break;
	case 'X':
		RX -= 5;
		break;
	case 'y':
		RY += 5;
		break;
	case 'Y':
		RY -= 5;
		break;
	case 'z':
		RZ += 5;
		break;
	case 'Z':
		RZ -= 5;
		break;
	case 'p':
		xx += 0.05;
		break;
	case 'P':
		if (xx < 0.2)
			xx -= 0.02;
		else
			xx -= 0.05;
		break;
	// Điều khiển xe tải
	case 'w': // Tiến
		isFW = true;
		X_XeTai += cos(RY_XeTai * 3.1415926f / 180.0f) * speedFW_XeTai;
		Z_XeTai -= sin(RY_XeTai * 3.1415926f / 180.0f) * speedFW_XeTai;
		RY_BanhXe += 3;
		break;
	case 's': // Lùi
		isBK = true;
		X_XeTai -= cos(RY_XeTai * 3.1415926f / 180.0f) * speedBK_XeTai;
		Z_XeTai += sin(RY_XeTai * 3.1415926f / 180.0f) * speedBK_XeTai;
		RY_BanhXe -= 3;
		break;
	case 'a': // Tiến trái
		isFW_L = true;
		RY_XeTai += 1;
		X_XeTai += cos(RY_XeTai * 3.1415926f / 180.0f) * speedFW_L_XeTai;
		Z_XeTai -= sin(RY_XeTai * 3.1415926f / 180.0f) * speedFW_L_XeTai;
		RY_BanhXe += 3;
		break;
	case 'd': // Tiến phải
		isFW_R = true;
		RY_XeTai -= 1;
		X_XeTai += cos(RY_XeTai * 3.1415926f / 180.0f) * speedFW_R_XeTai;
		Z_XeTai -= sin(RY_XeTai * 3.1415926f / 180.0f) * speedFW_R_XeTai;
		RY_BanhXe += 3;
		break;
	case 'A': // Lùi trái
		isBK_L = true;
		RY_XeTai -= 2;
		X_XeTai -= cos(RY_XeTai * 3.1415926f / 180.0f) * speedBK_L_XeTai;
		Z_XeTai += sin(RY_XeTai * 3.1415926f / 180.0f) * speedBK_L_XeTai;
		RY_BanhXe -= 3;
		break;
	case 'D': // Lùi phải
		isBK_R = true;
		RY_XeTai += 2;
		X_XeTai -= cos(RY_XeTai * 3.1415926f / 180.0f) * speedBK_R_XeTai;
		Z_XeTai += sin(RY_XeTai * 3.1415926f / 180.0f) * speedBK_R_XeTai;
		RY_BanhXe -= 3;
		break;
	// Điều khiến tàu
	case 'i':   // Tiến
		X_Tau += cos(RY_Tau * 3.1415926f / 180.0f) * speedFW_Tau;
		Z_Tau -= sin(RY_Tau * 3.1415926f / 180.0f) * speedFW_Tau;
		isFW_Tau = true;
		break;
	case 'k': // Lùi
		X_Tau -= cos(RY_Tau * 3.1415926f / 180.0f) * speedBK_Tau;
		Z_Tau += sin(RY_Tau * 3.1415926f / 180.0f) * speedBK_Tau;
		isBK_Tau = true;
		break;
	case 'j': // Tiến trái
		RY_Tau += 0.06;
		X_Tau += cos(RY_Tau * 3.1415926f / 180.0f) * speedFW_L_Tau;
		Z_Tau -= sin(RY_Tau * 3.1415926f / 180.0f) * speedFW_L_Tau;
		isFW_L_Tau = true;
		break;
	case 'l': // Tiến phải
		RY_Tau -= 0.06;
		X_Tau += cos(RY_Tau * 3.1415926f / 180.0f) * speedFW_R_Tau;
		Z_Tau -= sin(RY_Tau * 3.1415926f / 180.0f) * speedFW_R_Tau;
		isFW_R_Tau = true;
		break;
	case 'J': // Lùi trái
		RY_Tau -= 0.06;
		X_Tau -= cos(RY_Tau * 3.1415926f / 180.0f) * speedBK_L_Tau;
		Z_Tau += sin(RY_Tau * 3.1415926f / 180.0f) * speedBK_L_Tau;
		isBK_L_Tau = true;
		break;
	case 'L': // Lùi phải
		RY_Tau += 0.06;
		X_Tau -= cos(RY_Tau * 3.1415926f / 180.0f) * speedBK_R_Tau;
		Z_Tau += sin(RY_Tau * 3.1415926f / 180.0f) * speedBK_R_Tau;
		isBK_R_Tau = true;
		break;
	case 'I': // Xoay trái tại chỗ
		RY_Tau += 0.1;
		break;
	case 'K': // Xoay phải tại chỗ
		RY_Tau -= 0.1;
		break;
	// Điều khiển cần cẩu
	case 'r': // Thả dây
		if (Y_Day < 9.6)
			Y_Day += 0.2;
		break;
	case 'R': // Kéo dây
		if (is_Moc) {
			is_Moved = true;
		}
		if (Y_Day > 0.5)
			Y_Day -= 0.2;
		break;
	case 'f': // Kéo dây vào
		if (XoayX > -5) {
			XoayX -= 0.2;
			XoayX_TAM = -XoayX;
		}
		is_Xoay_X = false;
		if (Z_Day < 17)
			Z_Day += 0.1;
		break;
	case 'F': // Kéo dây ra
		if (XoayX < 5) {
			XoayX += 0.2;
			XoayX_TAM = -XoayX;
		}
		is_Xoay_X = false;
		if (Z_Day > 0)
			Z_Day -= 0.1;
		break;
	case 't': // Xoay xxx
		if (XoayZ > -5) {
			XoayZ -= 0.2;
			XoayZ_TAM = -XoayZ;
		}
		is_Xoay_Z = false;
		RY_DAY += 0.5;
		if (RY_DAY > 360)
			RY_DAY -= 360;
		break;
	case 'T': // Xoay xxx
		if (XoayZ < 10) {
			XoayZ += 0.2;
			XoayZ_TAM = -XoayZ;
		}
		is_Xoay_Z = false;
		RY_DAY -= 0.5;
		if (RY_DAY > 360)
			RY_DAY -= 360;
		break;
	case '9': // Moc Hang
		if (abs(ToaDo_Moc[2] - ToaDo_ThungHang[2]) < 0.2 && abs(ToaDo_Moc[1] - ToaDo_ThungHang[1]) < 0.1 && abs(ToaDo_Moc[0] - ToaDo_ThungHang[0]) < 0.2) {
			is_Moc = true;
		}
		break;
	case '0': // Bỏ móc hàng
		is_Moc = false;
		break;



	case 'm':
		y_cua1 -= 0.1f;
		if (y_cua1 < 0) {
			y_cua1 = 0;
		}
		break;
	case 'M':
		y_cua1 += 0.1f;
		if (y_cua1 > 1) {
			y_cua1 = 1;
		}
		break;
	case 'n':
		y_cua2 -= 0.1f;
		if (y_cua2 < 0) {
			y_cua2 = 0;
		}
		break;
	case 'N':
		y_cua2 += 0.1f;
		if (y_cua2 > 1) {
			y_cua2 = 1;
		}
		break;
	case 'b':
		quatgioquay = !quatgioquay;
		break;

	case 'c':// tháº£ dÃ¢y cáº§n trá»¥c
		if (dodaiday <= 0.63) {
			dodaiday += 0.02;
		}

		break;
	case 'C'://thu dÃ¢y cáº§n trá»¥c
		if (dodaiday >= 0) {
			dodaiday -= 0.02;
		}
		break;
	case 'v'://di chuyá»ƒn moc truc ra ngoÃ i
		if (dichuyen < 0) {
			dichuyen += 0.02;
		}
		break;
	case 'V'://di chuyá»ƒn mÃ³c trá»¥c vÃ o trong

		if (dichuyen >= -1.7) {
			dichuyen -= 0.02;
		}
		break;
    }
}

GLfloat radius_V = 1, theta = 0, phi = 0;
const GLfloat dr = 5.0 * DegreesToRadians;

// Các tham số cho projection
GLfloat l = -1.0, r = 1.0;
GLfloat bottom = -1.0, top = 1.0;
GLfloat zNear = 1, zFar = 300;

void reshape(int width, int height)
{
	/*point4 eye(radius_V * sin(theta) * cos(phi),
		radius_V * sin(theta) * sin(phi),
		radius_V * cos(theta),
		1.0);
	point4 at(0.0, 0.0, 0.0, 1.0);
	vec4 up(0.0, 1.0, 0.0, 1.0);*/

	vec4 eye = vec4(30, 20, -20, 1);
	vec4 at = vec4(0.0, 0.0, -30, 1);
	vec4 up = vec4(0, 1, 0, 1);

	mat4 View = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, View);

	mat4 Projection = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, Projection);

	glViewport(0, 0, width, height);
}

void update(int x) {
	if (isFW && speedFW_XeTai < 0.3f)
		speedFW_XeTai += 0.02f;

	if (isBK && speedBK_XeTai < 0.1f)
		speedBK_XeTai += 0.01f;

	if (isFW_L && speedFW_L_XeTai < 0.03f)
		speedFW_L_XeTai += 0.005f;

	if (isFW_R && speedFW_R_XeTai < 0.03f)
		speedFW_R_XeTai += 0.01f;

	if (isBK_R && speedBK_R_XeTai < 0.03f)
		speedBK_R_XeTai += 0.005f;

	if (isBK_L && speedBK_L_XeTai < 0.03f)
		speedBK_L_XeTai += 0.005f;

	if (isFW_Tau && speedFW_Tau < 0.3f)
		speedFW_Tau += 0.005f;

	if (isBK_Tau && speedBK_Tau < 0.1f)
		speedBK_Tau += 0.001f;

	if (isFW_L_Tau && speedFW_L_Tau < 0.1f)
		speedFW_L_Tau += 0.001f;

	if (isFW_R_Tau && speedFW_R_Tau < 0.1f)
		speedFW_R_Tau += 0.001f;

	if (isBK_R_Tau && speedBK_R_Tau < 0.1f)
		speedBK_R_Tau += 0.001f;

	if (isBK_L_Tau && speedBK_L_Tau < 0.1f)
		speedBK_L_Tau += 0.001f;

	if (!isFW)
		speedFW_XeTai = 0.0f;

	if (!isBK)
		speedBK_XeTai = 0.0f;

	if (!isFW_L)
		speedFW_L_XeTai = 0.0f;

	if (!isFW_R)
		speedFW_R_XeTai = 0.0f;

	if (!isBK_R)
		speedBK_R_XeTai = 0.0f;

	if (!isBK_L)
		speedBK_L_XeTai = 0.0f;

	if (!isFW_Tau)
		speedFW_Tau = 0.0f;

	if (!isBK_Tau)
		speedBK_Tau = 0.0f;

	if (!isFW_L_Tau)
		speedFW_L_Tau = 0.0f;

	if (!isFW_R_Tau)
		speedFW_R_Tau = 0.0f;

	if (!isBK_L_Tau)
		speedBK_L_Tau = 0.0f;

	if (!isBK_R_Tau)
		speedBK_R_Tau = 0.0f;


	glutTimerFunc(300, update, 0);

}

void KeyUp(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
		isFW = false;
		break;
	case 's':
		isBK = false;
		break;
	case 'a':
		isFW_L = false;
		break;
	case 'd':
		isFW_R = false;
		break;
	case 'A':
		isBK_L = false;
		break;
	case 'D':
		isBK_R = false;
		break;
	case 'i':
		isFW_Tau = false;
		break;
	case 'k':
		isBK_Tau = false;
		break;
	case 'j':
		isFW_L_Tau = false;
		break;
	case 'l':
		isFW_R_Tau = false;
		break;
	case 'J':
		isBK_L_Tau = false;
		break;
	case 'L':
		isBK_R_Tau = false;
		break;
	case 't':
		is_Xoay_Z = true;
		break;
	case 'T':
		is_Xoay_Z = true;
		break;
	case 'f':
		is_Xoay_X = true;
		break;
	case 'F':
		is_Xoay_X = true;
		break;
	}
}

int main( int argc, char **argv )
{
	// main function: program starts here

    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize( 1000, 1000 );                 
	glutInitWindowPosition(100,150);               
    glutCreateWindow( "Drawing a Cube" );            

   
	glewInit();										

    generateGeometry( );                          
    initGPUBuffers( );							   
    shaderSetup( );                               

    glutDisplayFunc( display );                   
    glutKeyboardFunc( keyboard );
	glutKeyboardUpFunc( KeyUp );
	glutTimerFunc(200, update, 0);
	glutReshapeFunc(reshape);
	glutTimerFunc(FRAME_TIME, updateFrame, 0);

	glutMainLoop();
    return 0;
}
