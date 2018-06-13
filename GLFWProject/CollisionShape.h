#pragma once

class CollisionShape
{
public:
	CollisionShape();
	~CollisionShape();
};

class CollisionBox : public CollisionShape
{
public:
	CollisionBox() {}
	~CollisionBox() {}
};

class CollisionSphere : public CollisionShape
{
public:
	CollisionSphere() {}
	~CollisionSphere() {}
};

