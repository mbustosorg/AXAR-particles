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

void Orbit::updateHeadParticle(Entity* entity, int index, Particle* current,
							   vec3 offsetVector, vec3 rotationAxis,
							   float rotZ, float rotSpeed) {
	
	vec4 center = vec4();
	
	current->pos = center + vec4(entity->sphericalLocation + offsetVector, 1.0);	current->ppos = current->pos;
	current->color = entity->mColor;
	current->index = index;
	current->translation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	current->rotation = rotate(rotSpeed, rotationAxis);
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
			
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24    )), vec3(100.0f, 100.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 1)), vec3(100.0f, 0.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 2)), vec3(100.0f, 100.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 3)), vec3(100.0f, 100.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 4)), vec3(100.0f, 0.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 5)), vec3(100.0f, 100.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 6)), vec3(100.0f, 100.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 7)), vec3(100.0f, 0.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 8)), vec3(100.0f, 100.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 9)), vec3(100.0f, 100.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 10)), vec3(100.0f, 0.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 11)), vec3(100.0f, 100.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 12)), vec3(100.0f, 100.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 13)), vec3(100.0f, 0.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 14)), vec3(100.0f, 100.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 15)), vec3(100.0f, 100.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 16)), vec3(100.0f, 0.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 17)), vec3(100.0f, 100.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 18)), vec3(100.0f, 100.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 19)), vec3(100.0f, 0.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 20)), vec3(100.0f, 100.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 21)), vec3(100.0f, 100.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 22)), vec3(100.0f, 0.0f, 100.0f), rotationAxis, rotZ, rotSpeed);
			updateHeadParticle(entity->second, particleId, &(particleHeads.at(particleId * 24 + 23)), vec3(100.0f, 100.0f, 0.0f), rotationAxis, rotZ, rotSpeed);
			
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

