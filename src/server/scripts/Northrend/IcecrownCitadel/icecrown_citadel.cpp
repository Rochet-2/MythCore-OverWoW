/*
* Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "icecrown_citadel.h"

// Weekly quest support
//* Deprogramming (DONE)
//* Securing the Ramparts (DONE)
//* Residue Rendezvous (DONE)
//* Blood Quickening (DONE)
//* Respite for a Tormented Soul

enum Texts
{
    // Highlord Tirion Fordring (at Light's Hammer)
    SAY_TIRION_INTRO_1 = 0,
    SAY_TIRION_INTRO_2 = 1,
    SAY_TIRION_INTRO_3 = 2,
    SAY_TIRION_INTRO_4 = 3,
    SAY_TIRION_INTRO_H_5 = 4,
    SAY_TIRION_INTRO_A_5 = 5,

    // The Lich King (at Light's Hammer)
    SAY_LK_INTRO_1 = 0,
    SAY_LK_INTRO_2 = 1,
    SAY_LK_INTRO_3 = 2,
    SAY_LK_INTRO_4 = 3,
    SAY_LK_INTRO_5 = 4,

    // Highlord Bolvar Fordragon (at Light's Hammer)
    SAY_BOLVAR_INTRO_1 = 0,

    // High Overlord Saurfang (at Light's Hammer)
    SAY_SAURFANG_INTRO_1 = 15,
    SAY_SAURFANG_INTRO_2 = 16,
    SAY_SAURFANG_INTRO_3 = 17,
    SAY_SAURFANG_INTRO_4 = 18,

    // Muradin Bronzebeard (at Light's Hammer)
    SAY_MURADIN_INTRO_1 = 13,
    SAY_MURADIN_INTRO_2 = 14,
    SAY_MURADIN_INTRO_3 = 15,

    // Rotting Frost Giant
    EMOTE_DEATH_PLAGUE_WARNING = 0,
};

enum Spells
{
    // Rotting Frost Giant
    SPELL_DEATH_PLAGUE = 72879,
    SPELL_DEATH_PLAGUE_AURA = 72865,
    SPELL_RECENTLY_INFECTED = 72884,
    SPELL_DEATH_PLAGUE_KILL = 72867,
    SPELL_STOMP = 64652,
    SPELL_ARCTIC_BREATH = 72848,

    // Frost Freeze Trap
    SPELL_COLDFLAME_JETS = 70460,

    // Alchemist Adrianna
    SPELL_HARVEST_BLIGHT_SPECIMEN = 72155,
    SPELL_HARVEST_BLIGHT_SPECIMEN25 = 72162,

    SPELL_SPIRIT_ALARM_1            = 70536,
    SPELL_SPIRIT_ALARM_2            = 70546,
    SPELL_SPIRIT_ALARM_3            = 70545,
    SPELL_SPIRIT_ALARM_4            = 70547,
    SPELL_DARK_RECKONING            = 69483,
    SPELL_DARK_RECKONING_SIPHON_LIFE= 69482,
    SPELL_AURA_OF_DARKNESS            = 69491,
    SPELL_SEVERED_ESSENCE_10        = 71906,
    SPELL_SEVERED_ESSENCE_25        = 71942,
    //Severed essence spells
    SPELL_CLONE_PLAYER              = 57507,
    //Druid spells
    SPELL_CAT_FORM                  = 57655,
    SPELL_MANGLE                    = 71925,
    SPELL_RIP                       = 71926,
    //Warlock
    SPELL_CORRUPTION                = 71937,
    SPELL_SHADOW_BOLT               = 71936,
    SPELL_RAIN_OF_CHAOS             = 71965,
    //Shaman
    SPELL_REPLENISHING_RAINS        = 71956,
    SPELL_LIGHTNING_BOLT            = 71934,
    //Rouge
    SPELL_DISENGAGE                 = 57635,
    SPELL_FOCUSED_ATTACKS           = 71955,
    SPELL_SINISTER_STRIKE           = 57640,
    SPELL_EVISCERATE                = 71933,
    //Mage
    SPELL_FIREBALL                  = 71928,
    //Warior
    SPELL_BLOODTHIRST               = 71938,
    SPELL_HEROIC_LEAP               = 71961,
    //Dk
    SPELL_DEATH_GRIP                = 57602,
    SPELL_NECROTIC_STRIKE           = 71951,
    SPELL_PLAGUE_STRIKE             = 71924,
    //Priest
    SPELL_GREATER_HEAL              = 71931,
    SPELL_RENEW                     = 71932,
    //Paladin
    SPELL_CLEANSE                   = 57767,
    SPELL_FLASH_OF_LIGHT            = 71930,
    SPELL_RADIANCE_AURA             = 71953,
    //Hunter
    SPELL_SHOOT_10                  = 71927,
    SPELL_SHOOT_25                  = 72258,
};

enum Events
{
    // Highlord Tirion Fordring (at Light's Hammer)
    // The Lich King (at Light's Hammer)
    // Highlord Bolvar Fordragon (at Light's Hammer)
    // High Overlord Saurfang (at Light's Hammer)
    // Muradin Bronzebeard (at Light's Hammer)
    EVENT_TIRION_INTRO_2 = 1,
    EVENT_TIRION_INTRO_3 = 2,
    EVENT_TIRION_INTRO_4 = 3,
    EVENT_TIRION_INTRO_5 = 4,
    EVENT_LK_INTRO_1 = 5,
    EVENT_TIRION_INTRO_6 = 6,
    EVENT_LK_INTRO_2 = 7,
    EVENT_LK_INTRO_3 = 8,
    EVENT_LK_INTRO_4 = 9,
    EVENT_BOLVAR_INTRO_1 = 10,
    EVENT_LK_INTRO_5 = 11,
    EVENT_SAURFANG_INTRO_1 = 12,
    EVENT_TIRION_INTRO_H_7 = 13,
    EVENT_SAURFANG_INTRO_2 = 14,
    EVENT_SAURFANG_INTRO_3 = 15,
    EVENT_SAURFANG_INTRO_4 = 16,
    EVENT_SAURFANG_RUN = 17,
    EVENT_MURADIN_INTRO_1 = 18,
    EVENT_MURADIN_INTRO_2 = 19,
    EVENT_MURADIN_INTRO_3 = 20,
    EVENT_TIRION_INTRO_A_7 = 21,
    EVENT_MURADIN_INTRO_4 = 22,
    EVENT_MURADIN_INTRO_5 = 23,
    EVENT_MURADIN_RUN = 24,

    // Rotting Frost Giant
    EVENT_DEATH_PLAGUE = 25,
    EVENT_STOMP = 26,
    EVENT_ARCTIC_BREATH = 27,

    // Frost Freeze Trap
    EVENT_ACTIVATE_TRAP = 28,

    //Deathspeaker High Priest
    EVENT_DARK_RECKONING    = 29,
    //Val'kyr Herald
    EVENT_SEVERED_ESSENCE   = 30,

    //Druid spells
    EVENT_CAT_FORM          = 31,
    EVENT_MANGLE            = 32,
    EVENT_RIP               = 33,
    //Warlock
    EVENT_CORRUPTION        = 34,
    EVENT_SHADOW_BOLT       = 35,
    EVENT_RAIN_OF_CHAOS     = 36,
    //Shaman
    EVENT_REPLENISHING_RAINS= 37,
    EVENT_LIGHTNING_BOLT    = 38,
    EVENT_CAN_CAST_REPLENISHING_RAINS= 39,
    //Rogue
    EVENT_FOCUSED_ATTACKS   = 40,
    EVENT_SINISTER_STRIKE   = 41,
    EVENT_EVISCERATE        = 42,
    //Mage
    EVENT_FIREBALL          = 43,
    //Warrior
    EVENT_BLOODTHIRST       = 44,
    EVENT_HEROIC_LEAP       = 45,
    //Dead Knight
    EVENT_DEATH_GRIP        = 46,
    EVENT_NECROTIC_STRIKE   = 47,
    EVENT_PLAGUE_STRIKE     = 48,
    //Priest
    EVENT_GREATER_HEAL      = 49,
    EVENT_RENEW             = 50,
    //Paladin
    EVENT_CLEANSE           = 51,
    EVENT_FLASH_OF_LIGHT    = 52,
    EVENT_RADIANCE_AURA     = 53,
    EVENT_CAN_CAST_RADIANCE_AURA,
    //Hunter
    EVENT_SHOOT             = 54,
    EVENT_DISENGAGE         = 55,
    EVENT_CAN_CAST_DISENGAGE= 56,
};

enum DataTypesICC
{
    DATA_DAMNED_KILLS = 1,
    DATA_PLAYER_CLASS = 2,
};

// at Light's Hammer
class npc_highlord_tirion_fordring_lh : public CreatureScript
{
    public:
        npc_highlord_tirion_fordring_lh() : CreatureScript("npc_highlord_tirion_fordring_lh") { }

        struct npc_highlord_tirion_fordringAI : public ScriptedAI
        {
            npc_highlord_tirion_fordringAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
            {
            }

            void Reset()
            {
                events.Reset();
                _theLichKing = 0;
                _bolvarFordragon = 0;
                _factionNPC = 0;
                _damnedKills = 0;
            }

            // IMPORTANT NOTE: This is triggered from per-GUID scripts
            // of The Damned SAI
            void SetData(uint32 type, uint32 data)
            {
                if (type == DATA_DAMNED_KILLS && data == 1)
                {
                    if (++_damnedKills == 2)
                    {
                        if (Creature* theLichKing = me->FindNearestCreature(NPC_THE_LICH_KING_LH, 150.0f))
                        {
                            if (Creature* bolvarFordragon = me->FindNearestCreature(NPC_HIGHLORD_BOLVAR_FORDRAGON_LH, 150.0f))
                            {
                                if (Creature* factionNPC = me->FindNearestCreature(instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? NPC_SE_HIGH_OVERLORD_SAURFANG : NPC_SE_MURADIN_BRONZEBEARD, 50.0f))
                                {
                                    me->setActive(true);
                                    _theLichKing = theLichKing->GetGUID();
                                    theLichKing->setActive(true);
                                    _bolvarFordragon = bolvarFordragon->GetGUID();
                                    bolvarFordragon->setActive(true);
                                    _factionNPC = factionNPC->GetGUID();
                                    factionNPC->setActive(true);
                                }
                            }
                        }

                        if (!_bolvarFordragon || !_theLichKing || !_factionNPC)
                            return;

                        Talk(SAY_TIRION_INTRO_1);
                        events.ScheduleEvent(EVENT_TIRION_INTRO_2, 4000);
                        events.ScheduleEvent(EVENT_TIRION_INTRO_3, 14000);
                        events.ScheduleEvent(EVENT_TIRION_INTRO_4, 18000);
                        events.ScheduleEvent(EVENT_TIRION_INTRO_5, 31000);
                        events.ScheduleEvent(EVENT_LK_INTRO_1, 35000);
                        events.ScheduleEvent(EVENT_TIRION_INTRO_6, 51000);
                        events.ScheduleEvent(EVENT_LK_INTRO_2, 58000);
                        events.ScheduleEvent(EVENT_LK_INTRO_3, 74000);
                        events.ScheduleEvent(EVENT_LK_INTRO_4, 86000);
                        events.ScheduleEvent(EVENT_BOLVAR_INTRO_1, 100000);
                        events.ScheduleEvent(EVENT_LK_INTRO_5, 108000);

                        if (instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
                        {
                            events.ScheduleEvent(EVENT_SAURFANG_INTRO_1, 120000);
                            events.ScheduleEvent(EVENT_TIRION_INTRO_H_7, 129000);
                            events.ScheduleEvent(EVENT_SAURFANG_INTRO_2, 139000);
                            events.ScheduleEvent(EVENT_SAURFANG_INTRO_3, 150000);
                            events.ScheduleEvent(EVENT_SAURFANG_INTRO_4, 162000);
                            events.ScheduleEvent(EVENT_SAURFANG_RUN, 170000);
                        }
                        else
                        {
                            events.ScheduleEvent(EVENT_MURADIN_INTRO_1, 120000);
                            events.ScheduleEvent(EVENT_MURADIN_INTRO_2, 124000);
                            events.ScheduleEvent(EVENT_MURADIN_INTRO_3, 127000);
                            events.ScheduleEvent(EVENT_TIRION_INTRO_A_7, 136000);
                            events.ScheduleEvent(EVENT_MURADIN_INTRO_4, 144000);
                            events.ScheduleEvent(EVENT_MURADIN_INTRO_5, 151000);
                            events.ScheduleEvent(EVENT_MURADIN_RUN, 157000);
                        }
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (_damnedKills != 2)
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TIRION_INTRO_2:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                            break;
                        case EVENT_TIRION_INTRO_3:
                            Talk(SAY_TIRION_INTRO_2);
                            break;
                        case EVENT_TIRION_INTRO_4:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                            break;
                        case EVENT_TIRION_INTRO_5:
                            Talk(SAY_TIRION_INTRO_3);
                            break;
                        case EVENT_LK_INTRO_1:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_POINT_NOSHEATHE);
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_1);
                            break;
                        case EVENT_TIRION_INTRO_6:
                            Talk(SAY_TIRION_INTRO_4);
                            break;
                        case EVENT_LK_INTRO_2:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_2);
                            break;
                        case EVENT_LK_INTRO_3:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_3);
                            break;
                        case EVENT_LK_INTRO_4:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_4);
                            break;
                        case EVENT_BOLVAR_INTRO_1:
                            if (Creature* bolvarFordragon = ObjectAccessor::GetCreature(*me, _bolvarFordragon))
                            {
                                bolvarFordragon->AI()->Talk(SAY_BOLVAR_INTRO_1);
                                bolvarFordragon->setActive(false);
                            }
                            break;
                        case EVENT_LK_INTRO_5:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                            {
                                theLichKing->AI()->Talk(SAY_LK_INTRO_5);
                                theLichKing->setActive(false);
                            }
                            break;
                        case EVENT_SAURFANG_INTRO_1:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_1);
                            break;
                        case EVENT_TIRION_INTRO_H_7:
                            Talk(SAY_TIRION_INTRO_H_5);
                            break;
                        case EVENT_SAURFANG_INTRO_2:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_2);
                            break;
                        case EVENT_SAURFANG_INTRO_3:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_3);
                            break;
                        case EVENT_SAURFANG_INTRO_4:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_4);
                            break;
                        case EVENT_MURADIN_RUN:
                        case EVENT_SAURFANG_RUN:
                            if (Creature* factionNPC = ObjectAccessor::GetCreature(*me, _factionNPC))
                                factionNPC->GetMotionMaster()->MovePath(factionNPC->GetDBTableGUIDLow()*10, false);
                            me->setActive(false);
                            _damnedKills = 3;
                            break;
                        case EVENT_MURADIN_INTRO_1:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->AI()->Talk(SAY_MURADIN_INTRO_1);
                            break;
                        case EVENT_MURADIN_INTRO_2:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            break;
                        case EVENT_MURADIN_INTRO_3:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                            break;
                        case EVENT_TIRION_INTRO_A_7:
                            Talk(SAY_TIRION_INTRO_A_5);
                            break;
                        case EVENT_MURADIN_INTRO_4:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->AI()->Talk(SAY_MURADIN_INTRO_2);
                            break;
                        case EVENT_MURADIN_INTRO_5:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->AI()->Talk(SAY_MURADIN_INTRO_3);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap events;
            InstanceScript* const instance;
            uint64 _theLichKing;
            uint64 _bolvarFordragon;
            uint64 _factionNPC;
            uint16 _damnedKills;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_highlord_tirion_fordringAI>(creature);
        }
};

class npc_rotting_frost_giant : public CreatureScript
{
    public:
        npc_rotting_frost_giant() : CreatureScript("npc_rotting_frost_giant") { }

        struct npc_rotting_frost_giantAI : public ScriptedAI
        {
            npc_rotting_frost_giantAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_DEATH_PLAGUE, 15000);
                events.ScheduleEvent(EVENT_STOMP, urand(5000, 8000));
                events.ScheduleEvent(EVENT_ARCTIC_BREATH, urand(10000, 15000));
            }

            void JustDied(Unit* /*killer*/)
            {
                events.Reset();
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DEATH_PLAGUE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                            {
                                Talk(EMOTE_DEATH_PLAGUE_WARNING, target->GetGUID());
                                DoCast(target, SPELL_DEATH_PLAGUE);
                            }
                            events.ScheduleEvent(EVENT_DEATH_PLAGUE, 15000);
                            break;
                        case EVENT_STOMP:
                            DoCastVictim(SPELL_STOMP);
                            events.ScheduleEvent(EVENT_STOMP, urand(15000, 18000));
                            break;
                        case EVENT_ARCTIC_BREATH:
                            DoCastVictim(SPELL_ARCTIC_BREATH);
                            events.ScheduleEvent(EVENT_ARCTIC_BREATH, urand(26000, 33000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_rotting_frost_giantAI>(creature);
        }
};

class npc_frost_freeze_trap : public CreatureScript
{
    public:
        npc_frost_freeze_trap() : CreatureScript("npc_frost_freeze_trap") { }

        struct npc_frost_freeze_trapAI: public Scripted_NoMovementAI
        {
            npc_frost_freeze_trapAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case 1000:
                    case 11000:
                        events.ScheduleEvent(EVENT_ACTIVATE_TRAP, uint32(action));
                        break;
                    case ACTION_STOP_TRAPS:
                        me->RemoveAurasDueToSpell(SPELL_COLDFLAME_JETS);
                        events.CancelEvent(EVENT_ACTIVATE_TRAP);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_ACTIVATE_TRAP)
                {
                    DoCast(me, SPELL_COLDFLAME_JETS);
                    events.ScheduleEvent(EVENT_ACTIVATE_TRAP, 22000);
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_frost_freeze_trapAI>(creature);
        }
};

class npc_alchemist_adrianna : public CreatureScript
{
    public:
        npc_alchemist_adrianna() : CreatureScript("npc_alchemist_adrianna") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (!creature->FindCurrentSpellBySpellId(SPELL_HARVEST_BLIGHT_SPECIMEN) && !creature->FindCurrentSpellBySpellId(SPELL_HARVEST_BLIGHT_SPECIMEN25))
                if (player->HasAura(SPELL_ORANGE_BLIGHT_RESIDUE) && player->HasAura(SPELL_GREEN_BLIGHT_RESIDUE))
                    creature->CastSpell(creature, SPELL_HARVEST_BLIGHT_SPECIMEN, false);
            return false;
        }
};

class DeathPlagueTargetSelector
{
    public:
        DeathPlagueTargetSelector(Unit* _caster) : caster(_caster) {}

        bool operator()(Unit* unit)
        {
            if (unit == caster)
                return true;

            if (unit->GetTypeId() != TYPEID_PLAYER)
                return true;

            if (unit->HasAura(SPELL_RECENTLY_INFECTED) || unit->HasAura(SPELL_DEATH_PLAGUE_AURA))
                return true;

            return false;
        }

        Unit* caster;
};

class spell_frost_giant_death_plague : public SpellScriptLoader
{
    public:
        spell_frost_giant_death_plague() : SpellScriptLoader("spell_frost_giant_death_plague") { }

        class spell_frost_giant_death_plague_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_frost_giant_death_plague_SpellScript);

            bool Load()
            {
                failed = false;
                return true;
            }

            // First effect
            void CountTargets(std::list<Unit*>& unitList)
            {
                unitList.remove(GetCaster());
                failed = unitList.empty();
            }

            // Second effect
            void FilterTargets(std::list<Unit*>& unitList)
            {
                // Select valid targets for jump
                unitList.remove_if(DeathPlagueTargetSelector(GetCaster()));
                if (!unitList.empty())
                {
                    std::list<Unit*>::iterator itr = unitList.begin();
                    std::advance(itr, urand(0, unitList.size()-1));
                    Unit* target = *itr;
                    unitList.clear();
                    unitList.push_back(target);
                }

                unitList.push_back(GetCaster());
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (GetHitUnit() != GetCaster())
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_DEATH_PLAGUE_AURA, true);
                else if (failed)
                    GetCaster()->CastSpell(GetCaster(), SPELL_DEATH_PLAGUE_KILL, true);
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_frost_giant_death_plague_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_AREA_ALLY_SRC);
                OnUnitTargetSelect += SpellUnitTargetFn(spell_frost_giant_death_plague_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_AREA_ALLY_SRC);
                OnEffect += SpellEffectFn(spell_frost_giant_death_plague_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }

            bool failed;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_frost_giant_death_plague_SpellScript();
        }
};

class spell_icc_harvest_blight_specimen : public SpellScriptLoader
{
    public:
        spell_icc_harvest_blight_specimen() : SpellScriptLoader("spell_icc_harvest_blight_specimen") { }

        class spell_icc_harvest_blight_specimen_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_icc_harvest_blight_specimen_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetHitUnit()->RemoveAurasDueToSpell(uint32(GetEffectValue()));
            }

            void HandleQuestComplete(SpellEffIndex /*effIndex*/)
            {
                GetHitUnit()->RemoveAurasDueToSpell(uint32(GetEffectValue()));
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_icc_harvest_blight_specimen_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
                OnEffect += SpellEffectFn(spell_icc_harvest_blight_specimen_SpellScript::HandleQuestComplete, EFFECT_1, SPELL_EFFECT_QUEST_COMPLETE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_icc_harvest_blight_specimen_SpellScript();
        }
};

class at_icc_saurfang_portal : public AreaTriggerScript
{
    public:
        at_icc_saurfang_portal() : AreaTriggerScript("at_icc_saurfang_portal") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            InstanceScript* instance = player->GetInstanceScript();
            if (!instance || instance->GetBossState(DATA_DEATHBRINGER_SAURFANG) != DONE)
                return true;

            player->TeleportTo(631, 4126.35f, 2769.23f, 350.963f, 0.0f);

            if (instance->GetData(DATA_COLDFLAME_JETS) == NOT_STARTED)
            {
                // Process relocation now, to preload the grid and initialize traps
                player->GetMap()->PlayerRelocation(player, 4126.35f, 2769.23f, 350.963f, 0.0f);

                instance->SetData(DATA_COLDFLAME_JETS, IN_PROGRESS);
                std::list<Creature*> traps;
                GetCreatureListWithEntryInGrid(traps, player, NPC_FROST_FREEZE_TRAP, 120.0f);
                traps.sort(Trinity::ObjectDistanceOrderPred(player));
                bool instant = false;
                for (std::list<Creature*>::iterator itr = traps.begin(); itr != traps.end(); ++itr)
                {
                    (*itr)->AI()->DoAction(instant ? 1000 : 11000);
                    instant = !instant;
                }
            }
            return true;
        }
};

class at_icc_shutdown_traps : public AreaTriggerScript
{
    public:
        at_icc_shutdown_traps() : AreaTriggerScript("at_icc_shutdown_traps") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (instance->GetData(DATA_COLDFLAME_JETS) == IN_PROGRESS)
                    instance->SetData(DATA_COLDFLAME_JETS, DONE);
            return true;
        }
};

class at_icc_start_blood_quickening : public AreaTriggerScript
{
    public:
        at_icc_start_blood_quickening() : AreaTriggerScript("at_icc_start_blood_quickening") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (instance->GetData(DATA_BLOOD_QUICKENING_STATE) == NOT_STARTED)
                    instance->SetData(DATA_BLOOD_QUICKENING_STATE, IN_PROGRESS);
            return true;
        }
};

class npc_severed_essence : public CreatureScript
{
public:
    npc_severed_essence() : CreatureScript("npc_severed_essence") { }

    CreatureAI* GetAI(Creature* pCreature) const

    {
        return new npc_severed_essenceAI (pCreature);

    }

    struct npc_severed_essenceAI : public ScriptedAI
    {
        npc_severed_essenceAI(Creature *c) : ScriptedAI(c)
        {
            instance = me->GetInstanceScript();
            playerClass = 0;
        }

        void Reset()
        {
            events.Reset();
        }

        void IsSummonedBy(Unit* owner)
        {
            if (owner->GetTypeId() != TYPEID_UNIT || owner->GetEntry() != NPC_VALKYR_HERALD)
                return;
            uiOwnerId = owner->GetGUID();
        }

        void KilledUnit(Unit* victim)
        {
            if (Unit *newVictim = SelectTarget(SELECT_TARGET_RANDOM, 0, -5.0f))
                AttackStart(newVictim);
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (Unit *player = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
            {
                player->CastSpell(me, SPELL_CLONE_PLAYER, true);
                SetData(DATA_PLAYER_CLASS, player->getClass());
            }
            else
            {
                me->DespawnOrUnsummon();
                return;
            }
            ASSERT(playerClass != 0);
            events.Reset();
            switch (playerClass)
            {
                case CLASS_DRUID:
                {
                    events.ScheduleEvent(EVENT_CAT_FORM, 100);
                    events.ScheduleEvent(EVENT_MANGLE, 3000);
                    events.ScheduleEvent(EVENT_RIP, 8000);
                    break;
                }
                case CLASS_WARLOCK:
                {
                    events.ScheduleEvent(EVENT_CORRUPTION, 100);
                    events.ScheduleEvent(EVENT_SHADOW_BOLT, 3000);
                    events.ScheduleEvent(EVENT_RAIN_OF_CHAOS, 8000);
                    break;
                }
                case CLASS_SHAMAN:
                {
                    events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 3000);
                    bCanCastReplenishingRains = true;
                    break;
                }
                case CLASS_ROGUE:
                {
                    events.ScheduleEvent(EVENT_FOCUSED_ATTACKS, 10000);
                    events.ScheduleEvent(EVENT_SINISTER_STRIKE, 2000);
                    events.ScheduleEvent(EVENT_EVISCERATE, 8000);
                    break;
                }
                case CLASS_MAGE:
                {
                    events.ScheduleEvent(EVENT_FIREBALL, 100);
                    break;
                }
                case CLASS_WARRIOR:
                {
                    events.ScheduleEvent(EVENT_BLOODTHIRST, 5000);
                    events.ScheduleEvent(EVENT_HEROIC_LEAP, 8000);
                    break;
                }
                case CLASS_DEATH_KNIGHT:
                {
                    events.ScheduleEvent(EVENT_DEATH_GRIP, 100);
                    events.ScheduleEvent(EVENT_NECROTIC_STRIKE, 4000);
                    events.ScheduleEvent(EVENT_PLAGUE_STRIKE, 7000);
                    break;
                }
                case CLASS_HUNTER:
                {
                    events.ScheduleEvent(EVENT_SHOOT, 100);
                    bCanCastDisengage = true;
                    break;
                }
                case CLASS_PALADIN:
                {
                    events.ScheduleEvent(EVENT_FLASH_OF_LIGHT, 100);
                    events.ScheduleEvent(EVENT_CLEANSE, 3000);
                    bCanCastRadianceAura = true;
                    break;
                case CLASS_PRIEST:
                {
                    events.ScheduleEvent(EVENT_GREATER_HEAL, 100);
                    events.ScheduleEvent(EVENT_RENEW, 1000);
                    break;
                }
            }
            }
        }
        void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
        {
            switch (playerClass)
            {
                case CLASS_SHAMAN:
                {
                    if(HealthBelowPct(30) && bCanCastReplenishingRains)
                    {
                        events.ScheduleEvent(EVENT_REPLENISHING_RAINS, 100);
                        events.ScheduleEvent(EVENT_CAN_CAST_REPLENISHING_RAINS, 15000);
                        bCanCastReplenishingRains = false;
                    }
                }
                case CLASS_HUNTER:
                {
                    if (!bCanCastDisengage)
                        break;
                    std::list<HostileReference*> &players = me->getThreatManager().getThreatList();
                    if (players.empty())
                        break;
                    for (std::list<HostileReference*>::iterator it = players.begin(); it != players.end(); ++it)
                    {
                        if (Unit *curTarget = (*it)->getTarget())
                        {
                            if (me->GetDistance2d(curTarget) < 5.0f)
                            {
                                me->SetFacingToObject(curTarget);
                                DoCast(curTarget, SPELL_DISENGAGE);
                                bCanCastDisengage = false;
                                events.ScheduleEvent(EVENT_CAN_CAST_DISENGAGE, 15000);
                                break;
                            }
                        }
                    }
                    break;
                }
                case CLASS_PALADIN:
                    if(HealthBelowPct(30) && bCanCastRadianceAura)
                    {
                        events.ScheduleEvent(EVENT_RADIANCE_AURA, 100);
                        events.ScheduleEvent(EVENT_CAN_CAST_RADIANCE_AURA, 15000);
                        bCanCastRadianceAura = false;
                    }
            }
        }

        void SetData(uint32 type, uint32 data)
        {
            if (type == DATA_PLAYER_CLASS)
                playerClass = data;
        }

        void HandleDruidEvents()
        {
            switch (uint32 eventId = events.ExecuteEvent())
            {
                case EVENT_CAT_FORM:
                    DoCast(me, SPELL_CAT_FORM);
                    break;
                case EVENT_MANGLE:
                    DoCast(me->getVictim(), SPELL_MANGLE);
                    events.ScheduleEvent(EVENT_MANGLE, 2000);
                    break;
                case EVENT_RIP:
                    DoCast(me->getVictim(), SPELL_RIP);
                    events.ScheduleEvent(EVENT_RIP, 8000);
                    break;
            }
        }

        void HandleWarlockEvents()
        {
            switch (uint32 eventId = events.ExecuteEvent())
            {
                case EVENT_CORRUPTION:
                    if (Unit *pUnit = SelectTarget(SELECT_TARGET_RANDOM, 1, -5.0f, true, -SPELL_CORRUPTION))
                        DoCast(pUnit, SPELL_CORRUPTION);
                    events.ScheduleEvent(EVENT_CORRUPTION, 20000);
                    break;
                case EVENT_SHADOW_BOLT:
                    DoCast(me->getVictim(), SPELL_SHADOW_BOLT);
                    events.ScheduleEvent(EVENT_SHADOW_BOLT, 5000);
                    break;
                case EVENT_RAIN_OF_CHAOS:
                    DoCast(me->getVictim(), SPELL_RAIN_OF_CHAOS);
                    events.ScheduleEvent(EVENT_RAIN_OF_CHAOS, 18000);
                    break;
            }
        }

        void HandleShamanEvents()
        {
            switch (uint32 eventId = events.ExecuteEvent())
            {
                case EVENT_CAN_CAST_REPLENISHING_RAINS:
                    bCanCastReplenishingRains = true;
                    break;
                case EVENT_REPLENISHING_RAINS:
                    DoCast(me, SPELL_REPLENISHING_RAINS);
                    break;
                case EVENT_LIGHTNING_BOLT:
                    DoCast(me->getVictim(), SPELL_LIGHTNING_BOLT);
                    events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 4000);
                    break;

            }
        }

        void HandleRogueEvents()
        {
            switch (uint32 eventId = events.ExecuteEvent())
            {
                case EVENT_FOCUSED_ATTACKS:
                    if (Unit *pUnit = SelectTarget(SELECT_TARGET_RANDOM, 1, 5.0f, true, -SPELL_FOCUSED_ATTACKS))
                        DoCast(pUnit, SPELL_FOCUSED_ATTACKS);
                    events.ScheduleEvent(EVENT_FOCUSED_ATTACKS, 15000);
                    break;
                case EVENT_SINISTER_STRIKE:
                    DoCast(me->getVictim(), SPELL_SINISTER_STRIKE);
                    events.ScheduleEvent(EVENT_SINISTER_STRIKE, 1000);
                    break;
                case EVENT_EVISCERATE:
                    DoCast(me->getVictim(), SPELL_EVISCERATE);
                    events.ScheduleEvent(EVENT_EVISCERATE, 6000);
                    break;
            }
        }

        void HandleMageEvents()
        {
            switch (uint32 eventId = events.ExecuteEvent())
            {
                case EVENT_FIREBALL:
                    DoCast(me->getVictim(), SPELL_FIREBALL);
                    events.ScheduleEvent(EVENT_FIREBALL, 3500);
                    break;
            }
        }

        void HandleWarriorEvents()
        {
            switch (uint32 eventId = events.ExecuteEvent())
            {
                case EVENT_BLOODTHIRST:
                {
                    if (Unit *pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0, 5.0f, true))
                        DoCast(pUnit, SPELL_BLOODTHIRST);
                    events.ScheduleEvent(EVENT_BLOODTHIRST, 12000);
                    break;
                }
                case EVENT_HEROIC_LEAP:
                {
                    if (Unit *pUnit = SelectTarget(SELECT_TARGET_RANDOM, 1, 8.0f, true))
                        DoCast(pUnit, SPELL_HEROIC_LEAP);
                    events.ScheduleEvent(EVENT_HEROIC_LEAP, 45000);
                    break;
                }
            }
        }

        void HandleDeathKnightEvents()
        {
            switch (uint32 eventId = events.ExecuteEvent())
            {
                case EVENT_DEATH_GRIP:
                {
                    if (Unit *pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0, -5.0f, true))
                    {
                        DoCast(pUnit, EVENT_DEATH_GRIP);
                        me->getThreatManager().clearReferences();
                        AttackStart(pUnit);
                    }
                    events.ScheduleEvent(EVENT_DEATH_GRIP, 35000);
                    break;
                }
                case EVENT_NECROTIC_STRIKE:
                {
                    DoCast(me->getVictim(), SPELL_NECROTIC_STRIKE);
                    events.ScheduleEvent(EVENT_NECROTIC_STRIKE, 6000);
                    break;
                }
                case EVENT_PLAGUE_STRIKE:
                {
                    if (Unit *pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0, 5.0f, true))
                        DoCast(pUnit, SPELL_PLAGUE_STRIKE);
                    events.ScheduleEvent(EVENT_PLAGUE_STRIKE, 7000);
                    break;
                }

            }
        }

        void HandleHunterEvents()
        {
            switch (uint32 eventId = events.ExecuteEvent())
            {
                case EVENT_CAN_CAST_DISENGAGE:
                {
                    bCanCastDisengage = true;
                    break;
                }
                case EVENT_SHOOT:
                {
                    DoCast(me->getVictim(), RAID_MODE<uint32>(SPELL_SHOOT_10, SPELL_SHOOT_25, SPELL_SHOOT_10, SPELL_SHOOT_25));
                    events.ScheduleEvent(EVENT_SHOOT, 2000);
                    break;
                }
            }
        }

        void HandlePaladinEvents()
        {
            Creature *valkyrHerald;
            switch (uint32 eventId = events.ExecuteEvent())
            {
                case EVENT_CAN_CAST_RADIANCE_AURA:
                {
                    bCanCastRadianceAura = true;
                    break;
                }
                case EVENT_FLASH_OF_LIGHT:
                {
                    if ((valkyrHerald = me->FindNearestCreature(NPC_VALKYR_HERALD, 40.0f)) && urand(0, 1))
                    {
                        DoCast(valkyrHerald, SPELL_FLASH_OF_LIGHT);
                    }
                    else
                    {
                        std::list<Creature*> others;
                        GetCreatureListWithEntryInGrid(others, me, NPC_SEVERED_ESSENCE, 40.0f);
                        Unit *pMob = 0;
                        for (std::list<Creature*>::const_iterator itr = others.begin(); itr != others.end(); ++itr)
                            if (!pMob || pMob->GetHealthPct() < (*itr)->GetHealthPct())
                                pMob = (*itr);
                        if (!pMob)
                            pMob = valkyrHerald;
                        if (pMob)
                            DoCast(pMob, SPELL_FLASH_OF_LIGHT);
                    }
                    events.ScheduleEvent(EVENT_FLASH_OF_LIGHT, 5000);
                    break;
                }
                case EVENT_CLEANSE:
                {
                    if (valkyrHerald = me->FindNearestCreature(NPC_VALKYR_HERALD, 30.0f))
                        DoCast(valkyrHerald, SPELL_CLEANSE);
                    events.ScheduleEvent(EVENT_CLEANSE, 5000);
                    break;
                }
                case EVENT_RADIANCE_AURA:
                {
                    DoCast(me, SPELL_RADIANCE_AURA);
                    break;
                }
            }
        }

        void HandlePriestEvents()
        {
            Creature *valkyrHerald;
            switch (uint32 eventId = events.ExecuteEvent())
            {
                case EVENT_RENEW:
                {
                    if ((valkyrHerald = me->FindNearestCreature(NPC_VALKYR_HERALD, 40.0f)) && urand(0, 1))
                    {
                        DoCast(valkyrHerald, SPELL_RENEW);
                    }
                    else
                    {
                        std::list<Creature*> others;
                        GetCreatureListWithEntryInGrid(others, me, NPC_SEVERED_ESSENCE, 40.0f);
                        Unit *pMob = 0;
                        for (std::list<Creature*>::const_iterator itr = others.begin(); itr != others.end(); ++itr)
                            if (!((*itr)->HasAura(SPELL_RENEW)) && (!pMob || pMob->GetHealthPct() < (*itr)->GetHealthPct()))
                                pMob = (*itr);
                        if (!pMob)
                        {
                            Aura *pMobAura;
                            for (std::list<Creature*>::const_iterator itr = others.begin(); itr != others.end(); ++itr)
                            {
                                if (!pMob && (*itr)->HasAura(SPELL_RENEW))
                                {
                                    pMob = (*itr);
                                    continue;
                                }
                                if ((*itr)->HasAura(SPELL_RENEW) &&
                                    ((pMobAura = (*itr)->GetAura(SPELL_RENEW)) && pMobAura->GetDuration() < pMob->GetAura(SPELL_RENEW)->GetDuration()))
                                    pMob = (*itr);
                            }
                        }
                        if (!pMob)
                            pMob = valkyrHerald;
                        if (pMob)
                            DoCast(pMob, SPELL_RENEW);
                    }
                    events.ScheduleEvent(EVENT_RENEW, 5000);
                    break;
                }
                case EVENT_GREATER_HEAL:
                {
                    if ((valkyrHerald = me->FindNearestCreature(NPC_VALKYR_HERALD, 40.0f)) && urand(0, 1))
                    {
                        DoCast(valkyrHerald, SPELL_GREATER_HEAL);
                    }
                    else
                    {
                        std::list<Creature*> others;
                        GetCreatureListWithEntryInGrid(others, me, NPC_SEVERED_ESSENCE, 40.0f);
                        Unit *pMob = 0;
                        for (std::list<Creature*>::const_iterator itr = others.begin(); itr != others.end(); ++itr)
                            if (!pMob || pMob->GetHealthPct() < (*itr)->GetHealthPct())
                                pMob = (*itr);
                        if (!pMob)
                            pMob = valkyrHerald;
                        if (pMob)
                            DoCast(pMob, SPELL_GREATER_HEAL);
                    }
                    events.ScheduleEvent(EVENT_GREATER_HEAL, 5000);
                    break;
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;
            events.Update(diff);

            switch (playerClass)
            {
                case CLASS_DRUID:
                    HandleDruidEvents();
                    break;
                case CLASS_WARLOCK:
                    HandleWarlockEvents();
                case CLASS_SHAMAN:
                    HandleShamanEvents();
                case CLASS_ROGUE:
                    HandleRogueEvents();
                case CLASS_MAGE:
                    HandleMageEvents();
                case CLASS_WARRIOR:
                    HandleWarriorEvents();
                case CLASS_DEATH_KNIGHT:
                    HandleDeathKnightEvents();
                case CLASS_HUNTER:
                    HandleHunterEvents();
                case CLASS_PALADIN:
                    HandlePaladinEvents();
                case CLASS_PRIEST:
                    HandlePriestEvents();
                default:
                    break;
            }
            DoMeleeAttackIfReady();
        }
        private:
            uint8 playerClass;
            InstanceScript *instance;
            EventMap events;
            uint64 uiOwnerId;
            bool bCanCastReplenishingRains;
            bool bCanCastDisengage;
            bool bCanCastRadianceAura;
    };
};

class npc_deathspeaker_high_priest : public CreatureScript
{
public:
    npc_deathspeaker_high_priest() : CreatureScript("npc_deathspeaker_high_priest") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_deathspeaker_high_priestAI (pCreature);

    }

    struct npc_deathspeaker_high_priestAI : public ScriptedAI
    {

        npc_deathspeaker_high_priestAI(Creature *c) : ScriptedAI(c)
        {
        }

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoCast(me, SPELL_AURA_OF_DARKNESS);
            events.Reset();
            events.ScheduleEvent(EVENT_DARK_RECKONING, 10000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_DARK_RECKONING:
                    {
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,1, 35.0f, false, -SPELL_DARK_RECKONING))
                            DoCast(pTarget, SPELL_DARK_RECKONING);
                        events.ScheduleEvent(EVENT_DARK_RECKONING, 20000);
                        break;
                    }
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
        private:
            EventMap events;
    };
};
class spell_icc_dark_reckoning : public SpellScriptLoader
{
    public:
        spell_icc_dark_reckoning() : SpellScriptLoader("spell_icc_dark_reckoning") { }

        class spell_icc_dark_reckoning_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_icc_dark_reckoning_AuraScript);

            bool Validate(SpellEntry const* /*spell*/)
            {
                if (!sSpellStore.LookupEntry(SPELL_DARK_RECKONING) ||
                    !sSpellStore.LookupEntry(SPELL_DARK_RECKONING_SIPHON_LIFE))
                    return false;
                return true;
            }

            void PeriodicTick(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                Unit *caster = GetCaster(), *target = GetTarget();
                if (caster && caster->isAlive() && target && target->isAlive())
                    caster->CastSpell(target, SPELL_DARK_RECKONING_SIPHON_LIFE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_icc_dark_reckoning_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_icc_dark_reckoning_AuraScript();
        }
};
class npc_val_kyr_herald : public CreatureScript
{
public:
    npc_val_kyr_herald() : CreatureScript("npc_val_kyr_herald") { }

    CreatureAI* GetAI(Creature* pCreature) const

    {
        return new npc_val_kyr_heraldAI (pCreature);

    }

    struct npc_val_kyr_heraldAI : public ScriptedAI
    {

        npc_val_kyr_heraldAI(Creature *c) : ScriptedAI(c), summons(c)
        {
            instance = c->GetInstanceScript();
        }

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.Reset();
            events.ScheduleEvent(EVENT_SEVERED_ESSENCE, 8000);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }

        void JustDied (Unit *killer)
        {
            summons.DespawnAll();
        }

        void SpellHitTarget(Unit *pTarget, const SpellEntry *spell)
        {
            // Not good target or too many players
            if (pTarget->GetTypeId() != TYPEID_PLAYER || !pTarget->isAlive())
                return;
            // Summon clone
            me->SummonCreature(NPC_SEVERED_ESSENCE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation(),TEMPSUMMON_CORPSE_DESPAWN, 0);
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SEVERED_ESSENCE:
                    {
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0, 100.0f, true))
                        {
                            DoCast(me, RAID_MODE<uint32>(SPELL_SEVERED_ESSENCE_10, SPELL_SEVERED_ESSENCE_25, SPELL_SEVERED_ESSENCE_10, SPELL_SEVERED_ESSENCE_25));
                        }
                        events.ScheduleEvent(EVENT_SEVERED_ESSENCE, 40000);
                        break;
                    }
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
        private:
            InstanceScript *instance;
            EventMap events;
            SummonList summons;
    };
};

class spell_icc_spirit_alarm : public SpellScriptLoader
{
    public:
        spell_icc_spirit_alarm() : SpellScriptLoader("spell_icc_spirit_alarm") { }

        class spell_icc_spirit_alarm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_icc_spirit_alarm_SpellScript);

            void AwakenDeathboundWard(SpellEffIndex effIndex)
            {
                if (!GetTargetUnit() || GetTargetUnit()->GetTypeId() != TYPEID_PLAYER)
                    return;
                if (!GetCaster())
                    return;
                InstanceScript *instance = GetCaster()->GetInstanceScript();
                GameObject *spiritAlarm = NULL;
                Creature *pWard = NULL;
                switch (GetSpellInfo()->Id)
                {
                    case SPELL_SPIRIT_ALARM_1:
                        spiritAlarm = GetCaster()->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM1));
                        pWard = spiritAlarm->GetMap()->GetCreature(instance->GetData64(DATA_DEATHBOUND_WARD1));
                        //Preload Spirit Alarm traps near Lord Marrowgar
                        spiritAlarm->GetMap()->LoadGrid(-273.845f, 2220.22f);
                        spiritAlarm->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM3))->SetPhaseMask(1, true);
                        break;
                    case SPELL_SPIRIT_ALARM_2:
                        spiritAlarm = GetCaster()->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM2));
                        pWard = spiritAlarm->GetMap()->GetCreature(instance->GetData64(DATA_DEATHBOUND_WARD2));
                        //Preload Spirit Alarm traps near Lord Marrowgar
                        spiritAlarm->GetMap()->LoadGrid(-273.845f, 2220.22f);
                        spiritAlarm->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM4))->SetPhaseMask(1, true);
                        break;
                    case SPELL_SPIRIT_ALARM_3:
                        spiritAlarm = GetCaster()->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM3));
                        pWard = spiritAlarm->GetMap()->GetCreature(instance->GetData64(DATA_DEATHBOUND_WARD3));
                        break;
                    case SPELL_SPIRIT_ALARM_4:
                        spiritAlarm = GetCaster()->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM4));
                        pWard = spiritAlarm->GetMap()->GetCreature(instance->GetData64(DATA_DEATHBOUND_WARD4));
                        break;
                }
                spiritAlarm->setActive(false);
                if (pWard && pWard->isAlive())
                {
                    pWard->SetReactState(REACT_AGGRESSIVE);
                    pWard->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    pWard->AI()->AttackStart(GetTargetUnit());
                }
            }
            void Register()
            {
                OnEffect += SpellEffectFn(spell_icc_spirit_alarm_SpellScript::AwakenDeathboundWard, EFFECT_2, SPELL_EFFECT_SEND_EVENT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_icc_spirit_alarm_SpellScript();
        }
};

void AddSC_icecrown_citadel()
{
    new npc_highlord_tirion_fordring_lh();
    new npc_rotting_frost_giant();
    new npc_frost_freeze_trap();
    new npc_alchemist_adrianna();
    new spell_frost_giant_death_plague();
    new spell_icc_harvest_blight_specimen();
    new at_icc_saurfang_portal();
    new at_icc_shutdown_traps();
    new at_icc_start_blood_quickening();
    new spell_icc_spirit_alarm();
    new npc_deathspeaker_high_priest();
    new spell_icc_dark_reckoning();
    new npc_val_kyr_herald();
    new npc_severed_essence();
}

