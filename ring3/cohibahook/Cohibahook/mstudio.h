/************************************************************************/
/* File: mstudio.h                                                      */
/* Author: Valve, ReactiioN                                             */
/* Last Update: 25.02.2015                                              */
/* Game Version: Global Offensive                                       */
/************************************************************************/
#ifndef __SOURCESDK_MSTUDIO_H__
#define __SOURCESDK_MSTUDIO_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "main.h"

#define Assert( _exp ) ((void)0)

namespace ValveSDK
{
	namespace studio
	{
		/************************************************************************/
		/* Structure: mstudiobbox_t                                             */
		/************************************************************************/
		struct mstudiobbox_t
		{
			int				bone;
			int				group;
			Vector	bbmin;
			Vector	bbmax;
			int				szhitboxnameindex;
			int				unused[8];

			const char* pszHitboxName() const
			{
				if (szhitboxnameindex == 0)
					return "";

				return ((const char*)this) + szhitboxnameindex;
			}
		};
		/************************************************************************/
		/* Structure: mstudiohitboxset_t                                        */
		/************************************************************************/
		struct mstudiohitboxset_t
		{
			int	sznameindex;

			inline char* const pszName(void) const
			{
				return ((char*)this) + sznameindex;
			}

			int numhitboxes;
			int hitboxindex;

			inline mstudiobbox_t* GetHitbox(int i) const
			{
				return (mstudiobbox_t*)(((BYTE*)this) + hitboxindex) + i;
			};
		};
		/************************************************************************/
		/* Structure: studiohdr_t                                               */
		/************************************************************************/
		struct studiohdr_t
		{
			int					id;
			int					version;
			int					checksum;
			char				name[64];
			int					length;
			Vector		eyeposition;
			Vector		illumposition;
			Vector		hull_min;
			Vector		hull_max;
			Vector		view_bbmin;
			Vector		view_bbmax;
			int					flags;
			int					numbones;
			int					boneindex;
			int					RemapSeqBone(int iSequence, int iLocalBone) const;
			int					RemapAnimBone(int iAnim, int iLocalBone) const;
			int					numbonecontrollers;
			int					bonecontrollerindex;
			int					numhitboxsets;
			int					hitboxsetindex;


			mstudiohitboxset_t	*GetHitboxSet(int i) const
			{
				Assert(i >= 0 && i < numhitboxsets);
				return (mstudiohitboxset_t *)(((BYTE *)this) + hitboxsetindex) + i;
			};

			inline mstudiobbox_t *GetHitbox(int i, int set) const
			{
				mstudiohitboxset_t const *s = GetHitboxSet(set);
				if (!s)
					return NULL;

				return s->GetHitbox(i);
			};

			inline int			GetHitboxCount(int set) const
			{
				mstudiohitboxset_t const *s = GetHitboxSet(set);
				if (!s)
					return 0;

				return s->numhitboxes;
			};

			int					numlocalanim;
			int					localanimindex;
			int					numlocalseq;
			int					localseqindex;
			bool				SequencesAvailable() const;
			int					GetNumSeq() const;
			int					iRelativeAnim(int baseseq, int relanim) const;
			int					iRelativeSeq(int baseseq, int relseq) const;
			mutable int			activitylistversion;
			mutable int			eventsindexed;
			int					GetSequenceActivity(int iSequence);
			void				SetSequenceActivity(int iSequence, int iActivity);
			int					GetActivityListVersion(void);
			void				SetActivityListVersion(int version) const;
			int					GetEventListVersion(void);
			void				SetEventListVersion(int version);
			int					numtextures;
			int					textureindex;
			int					numcdtextures;
			int					cdtextureindex;

			inline char			*pCdtexture(int i) const
			{
				return (((char *)this) + *((int *)(((BYTE *)this) + cdtextureindex) + i));
			};

			int					numskinref;
			int					numskinfamilies;
			int					skinindex;

			inline short		*pSkinref(int i) const
			{
				return (short *)(((BYTE *)this) + skinindex) + i;
			};

			int					numbodyparts;
			int					bodypartindex;
			int					numlocalattachments;
			int					localattachmentindex;
			int					GetNumAttachments(void) const;
			int					GetAttachmentBone(int i);
			void				SetAttachmentBone(int iAttachment, int iBone);
			int					numlocalnodes;
			int					localnodeindex;
			int					localnodenameindex;

			inline char			*pszLocalNodeName(int iNode) const
			{
				Assert(iNode >= 0 && iNode < numlocalnodes); return (((char *)this) + *((int *)(((BYTE *)this) + localnodenameindex) + iNode));
			}

			inline BYTE			*pLocalTransition(int i) const
			{
				Assert(i >= 0 && i < (numlocalnodes * numlocalnodes)); return (BYTE *)(((BYTE *)this) + localnodeindex) + i;
			};

			int					EntryNode(int iSequence);
			int					ExitNode(int iSequence);
			char				*pszNodeName(int iNode);
			int					GetTransition(int iFrom, int iTo) const;
			int					numflexdesc;
			int					flexdescindex;
			int					numflexcontrollers;
			int					flexcontrollerindex;
			int					numflexrules;
			int					flexruleindex;
			int					numikchains;
			int					ikchainindex;
			int					nummouths;
			int					mouthindex;
			int					numlocalposeparameters;
			int					localposeparamindex;
			int					GetNumPoseParameters(void) const;
			int					GetSharedPoseParameter(int iSequence, int iLocalPose) const;
			int					surfacepropindex;

			inline char * const pszSurfaceProp(void) const
			{
				return ((char *)this) + surfacepropindex;
			}

			int					keyvalueindex;
			int					keyvaluesize;

			inline const char * KeyValueText(void) const
			{
				return keyvaluesize != 0 ? ((char *)this) + keyvalueindex : NULL;
			}

			int					numlocalikautoplaylocks;
			int					localikautoplaylockindex;
			int					GetNumIKAutoplayLocks(void) const;
			int					CountAutoplaySequences() const;
			int					CopyAutoplaySequences(unsigned short *pOut, int outCount) const;
			int					GetAutoplayList(unsigned short **pOut) const;
			float				mass;
			int					contents;
			int					numincludemodels;
			int					includemodelindex;
			const studiohdr_t	*FindModel(void **cache, char const *modelname) const;
			mutable void		*virtualModel;
			int					szanimblocknameindex;

			inline char * const pszAnimBlockName(void) const
			{
				return ((char *)this) + szanimblocknameindex;
			}

			int					numanimblocks;
			int					animblockindex;
			mutable void		*animblockModel;
			BYTE *				GetAnimBlock(int i) const;
			int					bonetablebynameindex;

			inline const BYTE	*GetBoneTableSortedByName() const
			{
				return (BYTE *)this + bonetablebynameindex;
			}

			void				*pVertexBase;
			void				*pIndexBase;
			BYTE				constdirectionallightdot;
			BYTE				rootLOD;
			BYTE				numAllowedRootLODs;
			BYTE				unused[1];
			int					unused4;
			int					numflexcontrollerui;
			int					flexcontrolleruiindex;
			int					unused3[2];
			int					studiohdr2index;
			int					unused2[1];

			studiohdr_t(void)
			{
			}

		private:
			studiohdr_t(const studiohdr_t& vOther);
			friend struct virtualmodel_t;
		};
	}
}

#endif