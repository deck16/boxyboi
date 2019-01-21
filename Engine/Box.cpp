#include "Box.h"
IndexedTriangleList<Vec2> Box::model;

std::vector<std::unique_ptr<Box>> Box::SpawnBoxy(b2World& world)
{
	std::vector<std::unique_ptr<Box>> v;
	Vec2 parent_pos = GetPosition();
	Vec2 pos[4] = { parent_pos };

	pos[0] = GetWorldPoint(Vec2(-0.25f*size, -0.25f*size));
	pos[1] = GetWorldPoint(Vec2(0.25f*size, -0.25f*size));
	pos[2] = GetWorldPoint(Vec2(-0.25f*size, 0.25f*size));
	pos[3] = GetWorldPoint(Vec2(0.25f*size, 0.25f*size));
	for (int i = 0; i < 4; i++)
	{
		auto pColorTraitBoxy = pColorTrait->Clone();
		v.emplace_back(std::make_unique<Box>(std::move(pColorTraitBoxy),
			world, pos[i], size*0.5f, GetAngle(), GetVelocity(), GetAngularVelocity()));
	}
	return v;
}


std::unique_ptr<Box> Box::Spawn( float size,const Boundaries& bounds,b2World& world,std::mt19937& rng )
{
	std::uniform_real_distribution<float> pos_dist(
		-bounds.GetSize() + size * 2.0f,
		bounds.GetSize() - size * 2.0f
	);
	std::uniform_real_distribution<float> power_dist( 0.0f,6.0f );
	std::uniform_real_distribution<float> angle_dist( -PI,PI );
	std::uniform_int_distribution<int> type_dist( 0,4 );

	const auto linVel = (Vec2{ 1.0f,0.0f } * Mat2::Rotation( angle_dist( rng ) )) * power_dist( rng );
	const auto pos = Vec2{ pos_dist( rng ),pos_dist( rng ) };
	const auto ang = angle_dist( rng );
	const auto angVel = angle_dist( rng ) * 1.5f;

	std::unique_ptr<ColorTrait> pColorTrait;
	switch( type_dist( rng ) )
	{
	case 0:
		pColorTrait = std::make_unique<RedTrait>();
		break;
	case 1:
		pColorTrait = std::make_unique<GreenTrait>();
		break;
	case 2:
		pColorTrait = std::make_unique<BlueTrait>();
		break;
	case 3:
		pColorTrait = std::make_unique<WhiteTrait>();
		break;
	case 4:
		pColorTrait = std::make_unique<YellowTrait>();
		break;
	}
	
	return std::make_unique<Box>( std::move( pColorTrait ),world,pos,size,ang,linVel,angVel );
}