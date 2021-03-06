using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.ComponentModel;
using System.Text;

namespace mywowNet
{

    public partial class M2SceneNode : SceneNode
    {
        //animation player
         class M2AnimationPlayer : IAnimationPlayer
         {
             public M2AnimationPlayer(M2SceneNode node)
             {
                 _node = node;
             }

             M2SceneNode _node;

             public int CurrentAnimationIndex
             {
                 get { return M2SceneNode_getCurrentAnimationIndex(_node.pointer); }
             }

             public bool IsPlaying
             {
                 get { return M2SceneNode_isPlaying(_node.pointer); }
             }

             public bool Loop
             {
                 get { return M2SceneNode_isLoop(_node.pointer); }
                 set { M2SceneNode_setLoop(_node.pointer, value); }
             }

             public float CurrentFrame 
             {
                 get { return M2SceneNode_getCurrentFrame(_node.pointer);  }
                 set { M2SceneNode_setCurrentFrame(_node.pointer, value);  }         
             }

             public void Pause() { M2SceneNode_pause(_node.pointer);  }
             public void Resume() { M2SceneNode_resume(_node.pointer); }
             public void Stop() { M2SceneNode_stop(_node.pointer); }
             public void Step(float frame) { M2SceneNode_step(_node.pointer, frame); }
         }

        public M2SceneNode(IntPtr raw) : base(raw)
        {
            _animationPlayer = new M2AnimationPlayer(this);
        }

        public M2Type Type
        {
            get { return (M2Type)M2SceneNode_getType(pointer);  }
        }

        public SFileM2 FileM2
        {
            get 
            {
                SFileM2 m2 = new SFileM2();
                M2SceneNode_getFileM2(pointer, out m2);
                return m2;
            }
        }

        SRenderFlag[] _renderFlags;
        public SRenderFlag[] RenderFlags
        {
            get
            {
                uint num = FileM2.numRenderFlags;
                _renderFlags = new SRenderFlag[num];
                for (uint i = 0; i < num; ++i)
                {
                    M2SceneNode_getRenderFlag(pointer, out _renderFlags[i], i);
                }
                return _renderFlags;
            }
        }

        SGeoset[] _geosets;
        public SGeoset[] Geosets
        {
            get
            {
                uint num = FileM2.numGeosets;
                _geosets = new SGeoset[num];
                for (uint i = 0; i < num; ++i)
                {
                    M2SceneNode_getGeoset(pointer, i, out _geosets[i]);
                }
                return _geosets;
            }
        }

        List<string> lstTextureName = new List<string>();
        public string[] TextureNames
        {
            get
            {
                uint num = FileM2.numTextures;
                lstTextureName.Clear();
                StringBuilder path = new StringBuilder(256);
                for (uint i=0; i<num; ++i)
                {
                    M2SceneNode_getTextureFileName(pointer, i, path, 256);
                    if (path.Length > 0)
                        lstTextureName.Add(path.ToString(0, path.Length));                 
                }
                return lstTextureName.ToArray();
            }
        }

        public bool IsNpc
        {
            get { return M2SceneNode_isNpc(pointer); }
        }

        public SCharFeature CharFeature
        {
            get 
            {
                SCharFeature feature = new SCharFeature();
                M2SceneNode_getCharFeature(pointer, out feature);
                return feature;
            }
            set { M2SceneNode_setCharFeature(pointer, value);  }
        }

        public M2SceneNode HeadSceneNode
        {
            get
            {
                SM2ChildSceneNodes childSceneNodes = new SM2ChildSceneNodes();
                if (!M2SceneNode_getChildSceneNodes(pointer, out childSceneNodes))
                    return null;
                if (childSceneNodes.head != IntPtr.Zero)
                {
                    return new M2SceneNode(childSceneNodes.head);
                }
                return null;
            }
        }

        public M2SceneNode LeftShoulderSceneNode
        {
            get
            {
                SM2ChildSceneNodes childSceneNodes = new SM2ChildSceneNodes();
                if (!M2SceneNode_getChildSceneNodes(pointer, out childSceneNodes))
                    return null;
                if (childSceneNodes.leftShoulder != IntPtr.Zero)
                {
                    return new M2SceneNode(childSceneNodes.leftShoulder);
                }
                return null;
            }
        }

        public M2SceneNode RightShoulderSceneNode
        {
            get
            {
                SM2ChildSceneNodes childSceneNodes = new SM2ChildSceneNodes();
                if (!M2SceneNode_getChildSceneNodes(pointer, out childSceneNodes))
                    return null;
                if (childSceneNodes.rightShoulder != IntPtr.Zero)
                {
                    return new M2SceneNode(childSceneNodes.rightShoulder);
                }
                return null;
            }
        }

        public M2SceneNode LeftHandSceneNode
        {
            get
            {
                SM2ChildSceneNodes childSceneNodes = new SM2ChildSceneNodes();
                if (!M2SceneNode_getChildSceneNodes(pointer, out childSceneNodes))
                    return null;
                if (childSceneNodes.leftHand != IntPtr.Zero)
                {
                    return new M2SceneNode(childSceneNodes.leftHand);
                }
                return null;
            }
        }

        public M2SceneNode RightHandSceneNode
        {
            get
            {
                SM2ChildSceneNodes childSceneNodes = new SM2ChildSceneNodes();
                if (!M2SceneNode_getChildSceneNodes(pointer, out childSceneNodes))
                    return null;
                if (childSceneNodes.rightHand != IntPtr.Zero)
                {
                    return new M2SceneNode(childSceneNodes.rightHand);
                }
                return null;
            }
        }

        public string GetReplaceTextureFileName(E_MODEL_REPLACE_TEXTURE texture)
        {
            StringBuilder path = new StringBuilder(256);
            M2SceneNode_getReplaceTextureFileName(pointer, texture, path, 256);
            return path.ToString(0, path.Length);
        }

        public string GetRegionTextureFileName(ECharRegions region)
        {
            StringBuilder path = new StringBuilder(256);
            M2SceneNode_getRegionTextureFileName(pointer, region, path, 256);
            return path.ToString(0, path.Length);
        }

        public bool SetCamera(int camId)
        {
            return M2SceneNode_setModelCamera(pointer, camId); 
        }

        public int GetCamera()
        {
            return M2SceneNode_getModelCamera(pointer);
        }

        public bool TakeItem(int itemId, out int slot)
        {
           return M2SceneNode_takeItem(pointer, itemId, out slot);
        }

        public void ResetSlot(E_CHAR_SLOTS slot)
        {
            M2SceneNode_resetSlot(pointer, (int)slot);
        }

        public int GetSlotItemId(E_CHAR_SLOTS slot)
        {
            return M2SceneNode_getSlotItemId(pointer, (int)slot);
        }

        public bool SlotHasModel(E_CHAR_SLOTS slot)
        {
            return M2SceneNode_slotHasModel(pointer, (int)slot);
        }

        public void LoadStartOutfitId(int startid, bool deathknight)
        {
            M2SceneNode_loadStartOutfit(pointer, startid, deathknight);
        }

        public void LoadSet(int setid)
        {
            M2SceneNode_loadSet(pointer, setid);
        }

        public void BuildVisibleGeosets()
        {
            M2SceneNode_buildVisibleGeosets(pointer);
        }

        public void UpdateCharacter()
        {
            M2SceneNode_updateCharacter(pointer);
        }

        public bool UpdateNpc(int npcid)
        {
            return M2SceneNode_updateNpc(pointer, npcid);
        }

        public bool PlayAnimation(string name, uint subIdx, bool loop, int timeblend)
        {
            return M2SceneNode_playAnimationByName(pointer, name, subIdx, loop, timeblend);
        }

        public bool PlayAnimation(uint anim, bool loop, int timeblend)
        {
            return M2SceneNode_playAnimationByIndex(pointer, anim, loop, timeblend);
        }

        public uint AnimationCount
        {
            get { return M2SceneNode_getAnimationCount(pointer); }
        }

        public SAnimation? GetAnimation(uint index)
        {
            SAnimation v = new SAnimation();
            if (M2SceneNode_getAnimation(pointer, index, out v))
                return v;
            return null;
        }
        
        //animation
        private IAnimationPlayer _animationPlayer;
        public IAnimationPlayer AnimationPlayer { get { return _animationPlayer;  } }
  

        public void ShowModelPart(E_MODEL_PART modelpart, bool show)
        {
            M2SceneNode_showModelPart(pointer, modelpart, show);
        }

        public void ShowEditPart(E_MODEL_EDIT_PART editpart, bool show)
        {
            M2SceneNode_showEditPart(pointer, editpart, show);
        }

        public void ShowGeoset(int index, bool show)
        {
            M2SceneNode_showGeoset(pointer, (uint)index, show);
        }

        public bool IsGeosetShow(int index)
        {
            return M2SceneNode_isGeosetShow(pointer, (uint)index);
        }

        public void SetWireFrame(E_OVERRIDE_WIREFRAME wireframe)
        {
            M2SceneNode_setWireframe(pointer, wireframe);
        }

        public float ModelAlpha
        {
            get { return M2SceneNode_getModelAlpha(pointer); }
            set { M2SceneNode_setModelAlpha(pointer, value); }
        }

        public SColor ModelColor
        {
            set { M2SceneNode_setModelColor(pointer, value); }
            get
            {
                SColor c = new SColor();
                M2SceneNode_getModelColor(pointer, out c);
                return c;
            }
        }

        public void ResetState()
        {
            M2SceneNode_resetState(pointer);
        }

        public bool IsStateValid(E_M2_STATES state)
        {
            return M2SceneNode_isStateValid(pointer, state);
        }

        public bool ChangeState(E_M2_STATES state)
        {
            return M2SceneNode_changeState(pointer, state);
        }
    }
}
