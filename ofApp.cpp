#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofEnableDepthTest();

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 50, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.66666666666666666);

	auto word = "5";
	int sample_count = 200;
	vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);

	for (int i = 0; i < 150; i++){

		int x = ofRandom(-300, 300);
		int y = ofRandom(-300, 300);
		int z = ofRandom(-300, 300);
		int deg_x = ofRandom(-360, 360);
		int deg_y = ofRandom(-360, 360);
		int deg_z = ofRandom(-360, 360);

		auto translate_location = glm::normalize(glm::vec3(x, y, z)) * ofRandom(540);
		auto len = (int)(glm::length(translate_location) + ofGetFrameNum() * 2) % 540;
		translate_location = glm::normalize(translate_location) * len;

		ofPushMatrix();
		ofTranslate(translate_location);
		ofRotateZ(deg_z);
		ofRotateY(deg_y);
		ofRotateX(deg_x);

		for (int word_index = 0; word_index < word_path.size(); word_index++) {

			vector<ofPolyline> outline = word_path[word_index].getOutline();
			for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

				outline[outline_index] = outline[outline_index].getResampledByCount(sample_count);
				vector<glm::vec3> vertices = outline[outline_index].getVertices();
				vector<glm::vec3> mesh_vertices;
				vector<glm::vec3> base_location_list;
				vector<glm::highp_mat4x4> rotate_vertices;

				for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

					auto base_location = glm::vec3(this->font.stringWidth(word) * 0.5, this->font.stringHeight(word) * 0.5, 0);
					auto location = vertices[vertices_index] - glm::vec2(this->font.stringWidth(word) * 0.5, this->font.stringHeight(word) * 0.5);

					mesh_vertices.push_back(location);
					base_location_list.push_back(base_location);
				}

				ofMesh face, line;
				line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
				for (int k = 0; k < mesh_vertices.size(); k++) {

					face.addVertex(base_location_list[k] + glm::vec3(mesh_vertices[k] - glm::vec3(0, 0, 5)));
					face.addVertex(base_location_list[k] + glm::vec3(mesh_vertices[k] + glm::vec3(0, 0, 5)));

					line.addVertex(base_location_list[k] + glm::vec3(mesh_vertices[k] - glm::vec3(0, 0, 5)));
					line.addVertex(base_location_list[k] + glm::vec3(mesh_vertices[k] + glm::vec3(0, 0, 5)));

					if (k > 0) {

						face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(face.getNumVertices() - 4);
						face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 3); face.addIndex(face.getNumVertices() - 4);

						line.addIndex(line.getNumVertices() - 1); line.addIndex(line.getNumVertices() - 3);
						line.addIndex(line.getNumVertices() - 2); line.addIndex(line.getNumVertices() - 4);
					}
				}

				face.addIndex(face.getNumVertices() - 1); face.addIndex(0); face.addIndex(1);
				face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(2);

				line.addIndex(line.getNumVertices() - 1); line.addIndex(1);
				line.addIndex(line.getNumVertices() - 2); line.addIndex(0);

				ofColor color;
				color.setHsb(ofRandom(255), 130, 255);
				ofSetColor(color);
				face.draw();

				ofSetColor(255);
				line.drawWireframe();
			}
		}

		ofPopMatrix();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}