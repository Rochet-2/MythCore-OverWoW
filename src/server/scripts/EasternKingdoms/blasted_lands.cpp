/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2011 Myth Project <http://bitbucket.org/sun/myth-core/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You can't share Myth Project's sources! Only for personal use.
 */

#include "ScriptPCH.h"

/*######
## npc_deathly_usher
######*/

#define GOSSIP_ITEM_USHER "I wish to to visit the Rise of the Defiler."

#define SPELL_TELEPORT_SINGLE           12885
#define SPELL_TELEPORT_SINGLE_IN_GROUP  13142
#define SPELL_TELEPORT_GROUP            27686

class npc_deathly_usher : public CreatureScript
{
public:
    npc_deathly_usher() : CreatureScript("npc_deathly_usher") {}

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer, SPELL_TELEPORT_SINGLE, true);
        }

        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pPlayer->GetQuestStatus(3628) == QUEST_STATUS_INCOMPLETE && pPlayer->HasItemCount(10757, 1))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_USHER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

        return true;
    }
};

/*######
## npc_fallen_hero_of_horde
######*/
#define GOSSIP_H_F1 "Why are you here?"
#define GOSSIP_H_F2 "Continue story..."

#define GOSSIP_ITEM_FALLEN "Continue..."

#define GOSSIP_ITEM_FALLEN1 "What could be worse than death?"
#define GOSSIP_ITEM_FALLEN2 "Subordinates?"
#define GOSSIP_ITEM_FALLEN3 "What are the stones of binding?"
#define GOSSIP_ITEM_FALLEN4 "You can count on me, Hero"
#define GOSSIP_ITEM_FALLEN5 "I shall"

class npc_fallen_hero_of_horde : public CreatureScript
{
public:
    npc_fallen_hero_of_horde() : CreatureScript("npc_fallen_hero_of_horde") {}

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
                pPlayer->SEND_GOSSIP_MENU(1392, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+11:
                pPlayer->SEND_GOSSIP_MENU(1411, pCreature->GetGUID());
                if (pPlayer->GetQuestStatus(2784) == QUEST_STATUS_INCOMPLETE)
                    pPlayer->AreaExploredOrEventHappens(2784);
                if (pPlayer->GetTeam() == ALLIANCE)
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->SEND_GOSSIP_MENU(1411, pCreature->GetGUID());
                }
                break;

            case GOSSIP_ACTION_INFO_DEF+2:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
                pPlayer->SEND_GOSSIP_MENU(1451, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+21:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
                pPlayer->SEND_GOSSIP_MENU(1452, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+22:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 23);
                pPlayer->SEND_GOSSIP_MENU(1453, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+23:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 24);
                pPlayer->SEND_GOSSIP_MENU(1454, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+24:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 25);
                pPlayer->SEND_GOSSIP_MENU(1455, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+25:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 26);
                pPlayer->SEND_GOSSIP_MENU(1456, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+26:
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->AreaExploredOrEventHappens(2801);
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(2784) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_H_F1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        if (pPlayer->GetQuestStatus(2801) == QUEST_STATUS_INCOMPLETE && pPlayer->GetTeam() == HORDE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_H_F2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

        if (pPlayer->GetQuestStatus(2801) == QUEST_STATUS_INCOMPLETE && pPlayer->GetTeam() == ALLIANCE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_H_F1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

        return true;
    }
};

void AddSC_blasted_lands()
{
    new npc_deathly_usher();
    new npc_fallen_hero_of_horde();
}