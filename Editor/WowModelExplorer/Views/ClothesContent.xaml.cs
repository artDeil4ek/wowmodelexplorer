using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using AvalonDock;
using WowModelExplorer.Services;
using WowModelExplorer.Controls;
using WowModelExplorer.Data;
using mywowNet;

namespace WowModelExplorer.Views
{
    /// <summary>
    /// Interaction logic for ClothesContent.xaml
    /// </summary>
    public partial class ClothesContent : DockableContent
    {
        public ClothesContent()
        {
            InitializeComponent();

            SubscribeEvents();
        }

        private void SubscribeEvents()
        {
            _itemsHead.ItemSelected += new Item_SelectedEventHandler(ItemContent_ItemSelected);
            _itemsShoulder.ItemSelected += new Item_SelectedEventHandler(ItemContent_ItemSelected);
            _itemsShirt.ItemSelected += new Item_SelectedEventHandler(ItemContent_ItemSelected);
            _itemsChest.ItemSelected += new Item_SelectedEventHandler(ItemContent_ItemSelected);
            _itemsBelt.ItemSelected += new Item_SelectedEventHandler(ItemContent_ItemSelected);
            _itemsPants.ItemSelected += new Item_SelectedEventHandler(ItemContent_ItemSelected);
            _itemsBoots.ItemSelected += new Item_SelectedEventHandler(ItemContent_ItemSelected);
            _itemsBracers.ItemSelected += new Item_SelectedEventHandler(ItemContent_ItemSelected);
            _itemsGloves.ItemSelected += new Item_SelectedEventHandler(ItemContent_ItemSelected);
            _itemsCapes.ItemSelected += new Item_SelectedEventHandler(ItemContent_ItemSelected);
            _itemsTabards.ItemSelected += new Item_SelectedEventHandler(ItemContent_ItemSelected);
        }

        public void Initialize()
        {
            _itemsHead.FillData(E_CHAR_SLOTS.CS_HEAD);
            _itemsShoulder.FillData(E_CHAR_SLOTS.CS_SHOULDER);
            _itemsShirt.FillData(E_CHAR_SLOTS.CS_SHIRT);
            _itemsChest.FillData(E_CHAR_SLOTS.CS_CHEST);
            _itemsBelt.FillData(E_CHAR_SLOTS.CS_BELT);
            _itemsPants.FillData(E_CHAR_SLOTS.CS_PANTS);
            _itemsBoots.FillData(E_CHAR_SLOTS.CS_BOOTS);
            _itemsBracers.FillData(E_CHAR_SLOTS.CS_BRACERS);
            _itemsGloves.FillData(E_CHAR_SLOTS.CS_GLOVES);
            _itemsCapes.FillData(E_CHAR_SLOTS.CS_CAPE);
            _itemsTabards.FillData(E_CHAR_SLOTS.CS_TABARD);
        }

        private void ItemContent_ItemSelected(object sender, Item item)
        {
            int slot = 0;
            M2SceneNode node = ModelSceneService.Instance.MainM2SceneNode;
            if (node != null && node.Type == M2Type.MT_CHARACTER && !node.IsNpc)
            {
                node.TakeItem(item.Id, out slot);

                ModelEditorService.Instance.ShowModelPart(node);
                ModelEditorService.Instance.ShowEditPart(node);
                ModelEditorService.Instance.SetWireFrame(node);

                if (node.SlotHasModel((E_CHAR_SLOTS)slot))
                {
                    if (slot == (int)E_CHAR_SLOTS.CS_HEAD)
                    {
                        node.BuildVisibleGeosets();
                    }
                }
                else
                {
                    node.UpdateCharacter();
                }

                //update property
                ShellService.Instance.PropertyContent.UpdateCharacterNode(node);
                ShellService.Instance.EquipmentsWindow.Update();
            }
        }
    }
}
