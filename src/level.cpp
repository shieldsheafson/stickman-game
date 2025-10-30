#include "level.h"

// void Level::CheckCollisions(Player& player) {
//   for (const Terrain& terrain : mTerrain) {
//     Float2 playerMin = Float2(player.GetLeft(), player.GetBottom());
//     Float2 playerMax = Float2(player.GetRight(), player.GetTop());
//     Collision collision = Collides(playerMin, playerMax, terrain.GetMin(), terrain.GetMax());

//     switch (collision) {
//       case Collision::NONE:
//         player.SetOnGround(false);
//         break;

//       case Collision::GROUND:
//         player.SetBottom(terrain.GetTop());
//         player.SetVelocityY(0);
//         player.SetOnGround(true);
//         break;

//       case Collision::CEILING:
//         player.SetTop(terrain.GetBottom());
//         player.SetVelocityY(0);
//         player.SetOnGround(false);
//         break;

//       case Collision::LEFTWALL:
//         player.SetLeft(terrain.GetRight());
//         player.SetVelocityX(0);
//         break;

//       case Collision::RIGHTWALL:
//         player.SetRight(terrain.GetLeft());
//         player.SetVelocityX(0);
//         break;
//     }
//   }
// }