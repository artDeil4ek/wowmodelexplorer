using System;
using System.Collections;
using System.Collections.ObjectModel;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Media;

namespace FontDialog
{
 
    public partial class FontDialogBox : Window
    {
        public FontDialogBox()
        {
            InitializeComponent(); 

            this.fontFamilyListBox.ItemsSource = FontPropertyLists.FontFaces;
            this.fontStyleListBox.ItemsSource = FontPropertyLists.FontStyles;
            this.fontWeightListBox.ItemsSource = FontPropertyLists.FontWeights;
        }

        public new FontFamily FontFamily
        {
            get { return (FontFamily)this.fontFamilyListBox.SelectedItem; }  
            set {
                    this.fontFamilyListBox.SelectedItem = value;
                    this.fontFamilyListBox.ScrollIntoView(value);
                }
        }
        public new FontStyle FontStyle
        {
            get { return (FontStyle)this.fontStyleListBox.SelectedItem; }
            set
            {
                this.fontStyleListBox.SelectedItem = value;
                this.fontStyleListBox.ScrollIntoView(value);
            }
        }
        public new FontWeight FontWeight
        {
            get { return (FontWeight)this.fontWeightListBox.SelectedItem; }
            set
            {
                this.fontWeightListBox.SelectedItem = value;
                this.fontWeightListBox.ScrollIntoView(value);
            }
        }

        void okButton_Click(object sender, RoutedEventArgs e)
        {
            // Dialog box accepted
            this.DialogResult = true;
        }

        void fontFamilyTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            // If user enters family text, select family in list if matching item found
            this.FontFamily = new FontFamily(this.fontFamilyTextBox.Text);
        }
        void fontStyleTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            // If user enters style text, select style in list if matching item found
            if (FontPropertyLists.CanParseFontStyle(this.fontStyleTextBox.Text))
            {
                this.FontStyle = FontPropertyLists.ParseFontStyle(this.fontStyleTextBox.Text);
            }
        }
        void fontWeightTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            // If user enters weight text, select weight in list if matching item found
            if (FontPropertyLists.CanParseFontWeight(this.fontWeightTextBox.Text))
            {
                this.FontWeight = FontPropertyLists.ParseFontWeight(this.fontWeightTextBox.Text);
            }
        }
    }
}