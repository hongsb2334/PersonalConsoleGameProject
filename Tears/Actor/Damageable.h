#pragma once
#include <Actor/Actor.h>

namespace Craft
{
    class Damageable : public Craft::Actor
    {

        TYPE_DECLARATIONS(Damageable, Actor)

    public:
        Damageable(
            const std::string& image,
            const Vector2& position,
            Color color,
            int maxHp,
            int initialHp = -1
        );
        //데미지 처리 함수
        virtual void TakeDamage(int damage) final;
        //체력 회복 아이템 함수
        void Heal(int amount);
        //getter
        inline int GetHp() const { return hp; }
        inline int GetMaxHp() const { return maxHp; }
    private:
        //죽음 표시
        inline bool IsDead() const { return hp <= 0; }

    protected:
        //사망 시 처리
        virtual void OnDeath();

    private:
        int hp = 0;
        int maxHp = 0;
    };
}


