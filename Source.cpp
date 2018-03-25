#include <cmath>
#include <string>
#include "XLibrary11.hpp"

using namespace std;
using namespace XLibrary11;

enum MODE
{
    TITLE, GAME
};

int MAIN()
{
    const int blockNumber = 10;
    const float blockInterval = 200.0f;
    const float blockAmplitude = 200.0f;
    const float blockSpace = 200.0f;
    const float gravityAcceleration = 0.5f;
    const float jumpForce = 10.0f;
    const float speed = 2.0f;

    float blockOffset = 0.0f;
    float position = 0.0f;
    float gravity = 0.0f;
    int score = 0;
    MODE mode = TITLE;

    App::SetTitle(L"�͂΂�����");

    Camera camera;
    camera.color = Float4(0.5f, 0.75f, 1.0f, 1.0f);

    Sprite player(L"Player.png");
    player.scale = 3.0f;

    Sprite block(L"Block.png");
    block.scale = 5.0f;

    Text scoreText(L"0", 16);
    scoreText.position.y = 200.0f;
    scoreText.scale = 5.0f;
    scoreText.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

    Text title(L"�͂΂�����", 32);
    title.position.x = 100.0f;
    title.scale = 2.0f;
    title.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

    Float2 blockPosition[blockNumber];

    while (App::Refresh())
    {
        camera.Update();

        switch (mode)
        {
        case TITLE:

            if (App::GetKeyDown(VK_SPACE))
            {
                blockOffset = 0.0f;
                position = 0.0f;
                gravity = 0.0f;
                score = 0;
                mode = GAME;
                scoreText.Create(L"0", 16);

                for (int i = 0; i < blockNumber; i++)
                {
                    blockPosition[i] = Float2(blockOffset + blockInterval, rand() / (float)RAND_MAX * blockAmplitude - blockAmplitude / 2);
                    blockOffset = blockPosition[i].x;
                }
            }

            player.position = Float3(-200.0f, 0.0f, 0.0f);
            position = player.position.x;
            player.angles.z = 0.0f;
            player.Draw();

            title.Draw();

            scoreText.Draw();

            break;

        case GAME:

            for (int i = 0; i < blockNumber; i++)
            {
                if (blockPosition[i].x < -App::GetWindowSize().x)
                {
                    blockPosition[i] = Float2(blockOffset + blockInterval, rand() / (float)RAND_MAX * blockAmplitude - blockAmplitude / 2);
                    blockOffset = blockPosition[i].x;
                }

                float playerSize = player.GetSize().x * player.scale.x * 0.8f;
                float blockWidth = (block.GetSize().x * block.scale.x + playerSize) / 2.0f;
                float blockHeight = (blockSpace - playerSize) / 2.0f;

                if (blockPosition[i].x - blockWidth < player.position.x &&
                    blockPosition[i].x + blockWidth > player.position.x)
                {
                    if (blockPosition[i].y - blockHeight > player.position.y ||
                        blockPosition[i].y + blockHeight < player.position.y)
                    {
                        mode = TITLE;
                    }
                }

                blockPosition[i].x -= speed;

                block.position = blockPosition[i];
                block.position.y += (block.GetSize().y * block.scale.y + blockSpace) / 2.0f;
                block.Draw();

                block.position = blockPosition[i];
                block.position.y -= (block.GetSize().y * block.scale.y + blockSpace) / 2.0f;
                block.Draw();
            }

            blockOffset -= speed;


            gravity -= gravityAcceleration;

            if (App::GetKeyDown(VK_SPACE))
            {
                gravity = jumpForce;
            }

            player.position.y += gravity;
            player.angles.z = gravity * 5.0f;
            player.Draw();

            position += speed;

            if (position > blockInterval)
            {
                position -= blockInterval;
                score++;
                scoreText.Create(to_wstring(score), 16);
            }

            scoreText.Draw();

            break;
        }
    }

    return 0;
}