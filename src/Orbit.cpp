/*
 
 Copyright (C) 2017 Mauricio Bustos (m@bustos.org)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
*/

#include "Orbit.hpp"
#include "cinder/Rand.h"

/*
 
 mParticleBuffer[mDestinationIndex]->bind();
 size_t bufferSize = mParticleBuffer[mDestinationIndex]->getSize();
 mParticleBuffer[mDestinationIndex]->getBufferSubData(0, bufferSize, &tempData);
 for (int i = 0; i < particles.size(); i++) {
 particles[i] = tempData[i];
 }
 particles.at(0).pos.x = particles.at(0).pos.x + 1000.0;
 mParticleBuffer[mDestinationIndex]->bind();
 
 bufferSize = mHeadParticleBuffer[mDestinationIndex]->getSize();
 mHeadParticleBuffer[mDestinationIndex]->getBufferSubData(0, bufferSize, &tempHeadData);
 for (int i = 0; i < particleHeads.size(); i++) {
 particleHeads[i] = tempHeadData[i];
 }
 particleHeads.at(0).pos.x = particleHeads.at(0).pos.x + 1000.0;
 
 setupBuffers(mAttributes, mParticleBuffer, particles);
 setupBuffers(mHeadAttributes, mHeadParticleBuffer, particleHeads);
 

 */

Orbit::Orbit() {
	screenTime = 2.0f;
}

void Orbit::updateHeadParticle(int index, int total, Particle* current,
							   float theta, float thetaOffset,
							   float phi, float phiOffset,
							   float radius, float radiusOffset,
							   float rotZ, float rotSpeed) {
	
	vec4 center = vec4();
	
	float pX = (radius + radiusOffset) * cos(theta + thetaOffset) * sin(phi + phiOffset);  // Red
	float pY = (radius + radiusOffset) * sin(theta + thetaOffset) * sin(phi + phiOffset);  // Green
	float pZ = (radius + radiusOffset) * cos(phi + phiOffset);               // Blue
	
	float rX = radius * cos(theta) * sin(phi);  // Red
	float rY = radius * sin(theta) * sin(phi);  // Green
	float rZ = radius * cos(phi);               // Blue
	
	current->pos = (center + vec4(pX, pY, pZ, 1.0)) * rotate(-rotZ, vec3(rX, rY, 0.0f));
	current->rotation = rotate(rotSpeed, vec3(vec4(rY, -rX, 0.0f, 1.0) * rotate(-rotZ, vec3(rX, rY, 0.0f))));
	current->ppos = current->pos;
	current->color = Color(CM_HSV,
						   lmap<float>(index, 0.0f, total, 1.0f, 0.0f),
						   1.0f, 1.0f);
	current->index = index;
	current->translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
}

void Orbit::setup() {
	particles.assign(NUM_LINES * 2, Particle());
	particleHeads.assign(NUM_TRIANGLES * 3, Particle());
	
	vec4 center = vec4();
	
	for (int i = 0; i < particles.size(); i = i + TRAIL_LENGTH * 2)
	{
		int particleId = i / (TRAIL_LENGTH * 2);
		
		float rotSpeed = -Rand::randFloat(ROTATION_SPEED - 0.0008f, ROTATION_SPEED + 0.0008f);
		float rotZ = Rand::randFloat(-2.0f, 2.0f);
		
		float theta = (float)particleId / (float)NUM_PARTICLES * 2.0 * M_PI;
		float ringPhi = M_PI / 2.0;
		float sectionLength = 0.5f;
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24    )), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 1)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 2)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 3)), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 4)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 5)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 6)), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 7)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 8)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 9)), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 10)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 11)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 12)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 13)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 14)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 15)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 16)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 17)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 18)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 19)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 20)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 21)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 22)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
		updateHeadParticle(particleId, NUM_PARTICLES, &(particleHeads.at(particleId * 24 + 23)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
		
		for (int j = 0; j < TRAIL_LENGTH; j++) {
			
			float phi = M_PI / 2.0 - j * SECTION_ARC_LENGTH;
			float x = SPHERE_RADIUS * cos(theta) * sin(phi);  // Red
			float y = SPHERE_RADIUS * sin(theta) * sin(phi);  // Green
			float z = SPHERE_RADIUS * cos(phi);  // Blue
			
			auto &p = particles.at(i + j * 2);
			
			p.pos = (center + vec4(x, y, z, 1.0)) * rotate(-rotZ, vec3(x, y, 0));
			p.ppos = p.pos;
			p.color = Color(CM_HSV,
							lmap<float>(particleId, 0.0f, NUM_PARTICLES, 0.0f, 1.0f),
							1.0f, 1.0);
			//lmap<float>(j, 0.0f, TRAIL_LENGTH, 1.0f, 0.0f));
			p.index = i;
			p.translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
			p.rotation = rotate(rotSpeed, vec3(vec4(y, -x, 0.0f, 1.0) * rotate(-rotZ, vec3(x, y, 0.0f))));
			
			auto &pNext = particles.at(i + j * 2 + 1);
			
			phi = M_PI / 2.0 - (j + 1) * SECTION_ARC_LENGTH;
			x = SPHERE_RADIUS * cos(theta) * sin(phi);  // Red
			y = SPHERE_RADIUS * sin(theta) * sin(phi);  // Green
			z = SPHERE_RADIUS * cos(phi);  // Blue
			
			pNext.pos = (center + vec4(x, y, z, 1.0)) * rotate(-rotZ, vec3(x, y, 0));
			pNext.ppos = p.pos;
			pNext.color = Color(CM_HSV,
								lmap<float>(particleId, 0.0f, NUM_PARTICLES, 0.0f, 1.0f),
								1.0f, 1.0f);
			//lmap<float>(j, 0.0f, TRAIL_LENGTH, 1.0f, 0.0f));
			pNext.index = i;
			pNext.translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
			pNext.rotation = rotate(rotSpeed, vec3(vec4(y, -x, 0.0f, 1.0) * rotate(-rotZ, vec3(x, y, 0.0f))));
		}
	}
	
	// Load our update program.
	// Match up our attribute locations with the description we gave.
	
	mParticleRenderProg = gl::getStockShader(gl::ShaderDef().color());
	mParticleUpdateProg = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("orbitUpdate.vs"))
											   .feedbackFormat(GL_INTERLEAVED_ATTRIBS)
											   .feedbackVaryings({"position", "pposition", "color", "index", "translation", "rotation", "transition"})
											   .attribLocation("iPosition", 0)
											   .attribLocation("iColor", 1)
											   .attribLocation("iPPosition", 2)
											   .attribLocation("iIndex", 3)
											   .attribLocation("iTranslation", 4)
											   .attribLocation("iRotation", 8)
											   .attribLocation("iTransition", 12)
											   );
	mParticleHeadRenderProg = gl::getStockShader(gl::ShaderDef().color());
	mParticleHeadUpdateProg = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("orbitUpdate.vs"))
												   .feedbackFormat(GL_INTERLEAVED_ATTRIBS)
												   .feedbackVaryings({"position", "pposition", "color", "index", "translation", "rotation", "transition"})
												   .attribLocation("iPosition", 0)
												   .attribLocation("iColor", 1)
												   .attribLocation("iPPosition", 2)
												   .attribLocation("iIndex", 3)
												   .attribLocation("iTranslation", 4)
												   .attribLocation("iRotation", 8)
												   .attribLocation("iTransition", 12)
												   );
	
	setupBuffers(mAttributes, mParticleBuffer, particles);
	setupBuffers(mAttributesHead, mParticleHeadBuffer, particleHeads);
	
}

