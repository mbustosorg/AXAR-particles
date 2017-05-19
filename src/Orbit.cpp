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
	screenTime = 30.0f;
}

void Orbit::updateHeadParticle(Entity* entity, int index, unsigned long total, Particle* current,
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
	//float rZ = radius * cos(phi);               // Blue
	
	current->pos = vec4(entity->sphericalLocation, 1.0); //vec4(pX, pY, pZ, 1.0)) * rotate(-rotZ, vec3(rX, rY, 0.0f));
	current->rotation = rotate(rotSpeed, vec3(vec4(rY, -rX, 0.0f, 1.0) * rotate(-rotZ, vec3(rX, rY, 0.0f))));
	current->ppos = current->pos;
	current->color = entity->mColor;
	current->index = index;
	current->translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
}

void Orbit::setup() {
	
	particles.assign(Num_Lines * 2, Particle());
	particleHeads.assign(Num_Triangles * 3, Particle());
	
	vec4 center = vec4();
	
	int particleId = 0;
	for (unordered_map<string, Entity*>::iterator entity = mEntities.begin(); entity != mEntities.end(); ++entity) {
		
		vec3 sphericalLocation = entity->second->sphericalLocation;
		
		for (int i = particleId * (TRAIL_LENGTH * 2); i < (particleId + 1) * (TRAIL_LENGTH * 2); i = i + TRAIL_LENGTH * 2)	{
			
			float rotSpeed = -Rand::randFloat(ROTATION_SPEED - 0.008f, ROTATION_SPEED + 0.008f);
			float rotZ = Rand::randFloat(-1.5f, 1.5f);
			vec3 flatSphericalLocation = vec3(sphericalLocation.z, 0.0, -sphericalLocation.x);
			vec3 rotationAxis = vec3(vec4(flatSphericalLocation, 1.0) * rotate(-rotZ, sphericalLocation));
			
			float theta = entity->second->mLatitude / 180.0f * M_PI;//(float)particleId / (float)Num_Particles * 2.0 * M_PI;
			float ringPhi = M_PI / 2.0;
			float sectionLength = 0.5f;
			
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24    )), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 1)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 2)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 3)), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 4)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 5)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 6)), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 7)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 8)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 9)), theta, 0.0f, ringPhi, 0.0f, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 10)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 11)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 12)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 13)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 14)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, HEAD_DEPTH, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 15)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 16)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 17)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 18)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 19)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 20)), theta,  HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 21)), theta, 0.0f, ringPhi, - 2.0 * sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, 0.0f, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 22)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, + HEAD_DEPTH, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, Num_Particles, &(particleHeads.at(particleId * 24 + 23)), theta, -HEAD_ARC_WIDTH, ringPhi, - sectionLength * SECTION_ARC_LENGTH, SPHERE_RADIUS, - HEAD_DEPTH, rotZ, rotSpeed);
			
			for (int j = 0; j < TRAIL_LENGTH; j++) {

				auto &p = particles.at(i + j * 2);
				p.pos = (center + vec4(sphericalLocation, 1.0)) * rotate(-j * SECTION_ARC_LENGTH, rotationAxis);
				p.ppos = p.pos;
				p.color = entity->second->mColor;
				p.index = i;
				p.translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
				p.rotation = rotate(rotSpeed, rotationAxis);
				
				auto &pNext = particles.at(i + j * 2 + 1);
				pNext.pos = (center + vec4(sphericalLocation, 1.0)) * rotate(- (j + 1) * SECTION_ARC_LENGTH, rotationAxis);
				pNext.ppos = pNext.pos;
				pNext.color = entity->second->mColor;
				pNext.index = i;
				pNext.translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
				pNext.rotation = rotate(rotSpeed, rotationAxis);
			}
		}
		particleId++;
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

