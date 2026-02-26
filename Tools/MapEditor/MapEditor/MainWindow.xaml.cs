using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Media;
using Microsoft.Win32;

namespace MapEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private ObservableCollection<CellViewModel> _cells = new();
        private int _columns;
        private int _rows;

        // visual cell sizing used to force wrap width
        private const int CellSize = 28; // matches ToggleButton Width/Height in XAML
        private const int CellMargin = 2; // left+right approximated by ToggleButton Margin

        // Default tile size written into the file header. Change as needed.
        private const int DefaultTileSize = 1;

        // Drag state
        private bool _isDragging;
        private bool _dragSetState;

        public MainWindow()
        {
            InitializeComponent();
            CellItemsControl1.ItemsSource = _cells;

            // Attach pointer handlers so user can click-and-drag to paint cells
            CellItemsControl1.PreviewMouseLeftButtonDown += CellItemsControl_PreviewMouseLeftButtonDown;
            CellItemsControl1.PreviewMouseLeftButtonUp += CellItemsControl_PreviewMouseLeftButtonUp;
            CellItemsControl1.PreviewMouseMove += CellItemsControl_PreviewMouseMove;
            CellItemsControl1.LostMouseCapture += CellItemsControl_LostMouseCapture;
        }

        private void UpdateSizeButton_Click(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(MapSizeX1.Text, out var cols) || cols <= 0)
            {
                MessageBox.Show("Invalid Map Size X", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            if (!int.TryParse(MapSizeZ1.Text, out var rows) || rows <= 0)
            {
                MessageBox.Show("Invalid Map Size Z", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            _columns = cols;
            _rows = rows;
            CreateCells(cols, rows);
        }

        private void CreateCells(int columns, int rows)
        {
            _cells.Clear();
            var total = columns * rows;
            for (int i = 0; i < total; i++)
            {
                _cells.Add(new CellViewModel { IsOn = false });
            }

            // Force ItemsControl width so WrapPanel wraps exactly at 'columns' items per row.
            // account for margin/padding: approximate as CellSize + CellMargin
            var approxItemWidth = CellSize + CellMargin;
            CellItemsControl1.Width = columns * approxItemWidth;
        }

        private void CellToggle_Click(object? sender, RoutedEventArgs e)
        {
            // Single-click fallback: toggle the clicked cell.
            if (sender is ToggleButton tb && tb.DataContext is CellViewModel cvm)
            {
                cvm.IsOn = !cvm.IsOn;
                cvm.NotifyUpdated();
            }
        }

        // ---------- Drag handlers ----------

        private void CellItemsControl_PreviewMouseLeftButtonDown(object? sender, MouseButtonEventArgs e)
        {
            var tb = FindToggleButtonFromEventSource(e.OriginalSource as DependencyObject);
            if (tb is null) return;

            if (tb.DataContext is CellViewModel cvm)
            {
                // Toggle the clicked cell and begin drag mode.
                cvm.IsOn = !cvm.IsOn;
                cvm.NotifyUpdated();

                _isDragging = true;
                _dragSetState = cvm.IsOn;

                // capture to continue receiving mouse events even if cursor leaves the control
                CellItemsControl1.CaptureMouse();

                // prevent the normal Click event from firing (we handled toggle)
                e.Handled = true;
            }
        }

        private void CellItemsControl_PreviewMouseMove(object? sender, MouseEventArgs e)
        {
            if (!_isDragging || e.LeftButton != MouseButtonState.Pressed) return;

            // Use InputHitTest with coordinates relative to the ItemsControl (more reliable over ScrollViewer)
            var relativePoint = e.GetPosition(CellItemsControl1);
            var hitObj = CellItemsControl1.InputHitTest(relativePoint) as DependencyObject;
            if (hitObj is null) return;

            var tb = FindToggleButtonFromEventSource(hitObj);
            if (tb is null) return;

            if (tb.DataContext is CellViewModel cvm && cvm.IsOn != _dragSetState)
            {
                cvm.IsOn = _dragSetState;
                cvm.NotifyUpdated();
            }
        }

        private void CellItemsControl_PreviewMouseLeftButtonUp(object? sender, MouseButtonEventArgs e)
        {
            if (!_isDragging) return;

            _isDragging = false;
            if (CellItemsControl1.IsMouseCaptured) CellItemsControl1.ReleaseMouseCapture();
            e.Handled = true;
        }

        private void CellItemsControl_LostMouseCapture(object? sender, MouseEventArgs e)
        {
            // ensure drag state is cleared if capture is lost
            _isDragging = false;
        }

        private static ToggleButton? FindToggleButtonFromEventSource(DependencyObject? source)
        {
            var current = source;
            while (current != null)
            {
                if (current is ToggleButton tb) return tb;
                current = VisualTreeHelper.GetParent(current);
            }

            return null;
        }

        // ---------- Save logic ----------

        private void SaveMapDataButton_Click(object sender, RoutedEventArgs e)
        {
            if (_columns <= 0 || _rows <= 0)
            {
                MessageBox.Show("No map to save. Set size and press Update Size.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            // Determine tile size from the TileSize text box, fallback to DefaultTileSize
            int tileSizeValue = DefaultTileSize;
            if (!string.IsNullOrWhiteSpace(TileSize.Text) && int.TryParse(TileSize.Text, out var parsed) && parsed > 0)
            {
                tileSizeValue = parsed;
            }

            // Match C++ output:
            // Header: tileSize tilesX tilesZ\n
            // Then for each Z row, write tilesX characters '1' or '0' (no separators), then '\n'
            var sb = new StringBuilder();
            sb.Append(tileSizeValue);
            sb.Append(' ');
            sb.Append(_columns); // tilesX
            sb.Append(' ');
            sb.Append(_rows);    // tilesZ
            sb.AppendLine();

            for (int z = 0; z < _rows; z++)
            {
                for (int x = 0; x < _columns; x++)
                {
                    var index = z * _columns + x;
                    var ch = (index < _cells.Count && _cells[index].IsOn) ? '1' : '0';
                    sb.Append(ch);
                }

                sb.AppendLine();
            }

            // Prompt user for save location
            var sfd = new SaveFileDialog
            {
                Title = "Save map data",
                FileName = "mapdata.txt",
                DefaultExt = "txt",
                Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*"
            };

            var result = sfd.ShowDialog(this);
            if (result != true)
            {
                // User cancelled
                return;
            }

            try
            {
                var path = sfd.FileName;
                File.WriteAllText(path, sb.ToString());
                MessageBox.Show($"Map saved to: {path}", "Saved", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to save map: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        // ---------- Load logic ----------

        private void LoadMapDataButton_Click(object sender, RoutedEventArgs e)
        {
            var ofd = new OpenFileDialog
            {
                Title = "Open map data",
                DefaultExt = "txt",
                Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*"
            };

            if (ofd.ShowDialog(this) != true)
                return;

            try
            {
                using var sr = new StreamReader(ofd.FileName);
                var header = sr.ReadLine();
                if (string.IsNullOrWhiteSpace(header))
                {
                    MessageBox.Show("Invalid file format: empty header", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                var parts = header.Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length < 3
                    || !int.TryParse(parts[0], out var fileTileSize)
                    || !int.TryParse(parts[1], out var tilesX)
                    || !int.TryParse(parts[2], out var tilesZ))
                {
                    MessageBox.Show("Invalid file header. Expected: tileSize tilesX tilesZ", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                // Update UI sizes and create cells
                _columns = tilesX;
                _rows = tilesZ;
                MapSizeX1.Text = _columns.ToString();
                MapSizeZ1.Text = _rows.ToString();
                TileSize.Text = fileTileSize.ToString();
                CreateCells(_columns, _rows);

                // Read each Z row and parse 0/1 characters. Missing bits are treated as 0.
                for (int z = 0; z < _rows; z++)
                {
                    var line = sr.ReadLine() ?? string.Empty;
                    // accept either compact "0101" or lines that include whitespace; extract only 0/1 chars.
                    var bits = line.Where(c => c == '0' || c == '1').ToArray();

                    for (int x = 0; x < _columns; x++)
                    {
                        var index = z * _columns + x;
                        var isOn = x < bits.Length && bits[x] == '1';
                        if (index < _cells.Count)
                        {
                            _cells[index].IsOn = isOn;
                            _cells[index].NotifyUpdated();
                        }
                    }
                }

                MessageBox.Show($"Loaded map from: {ofd.FileName} (tileSize={fileTileSize})", "Loaded", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to load map: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        // ---------- Reset logic ----------

        private void ResetMapButton_Click(object sender, RoutedEventArgs e)
        {
            if (_cells.Count == 0)
            {
                MessageBox.Show("No map to reset. Set size and press Update Size.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            for (int i = 0; i < _cells.Count; i++)
            {
                if (_cells[i].IsOn)
                {
                    _cells[i].IsOn = false;
                    _cells[i].NotifyUpdated();
                }
            }
        }

        // ---------- Set All Access logic ----------

        private void ButtonSetAllAccess_Click(object sender, RoutedEventArgs e)
        {
            if (_cells.Count == 0)
            {
                MessageBox.Show("No map to update. Set size and press Update Size.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            for (int i = 0; i < _cells.Count; i++)
            {
                if (!_cells[i].IsOn)
                {
                    _cells[i].IsOn = true;
                    _cells[i].NotifyUpdated();
                }
            }
        }
    }

    public class CellViewModel : INotifyPropertyChanged
    {
        private bool _isOn;

        public bool IsOn
        {
            get => _isOn;
            set
            {
                if (_isOn == value) return;
                _isOn = value;
                OnPropertyChanged(nameof(IsOn));
                OnPropertyChanged(nameof(Display));
                OnPropertyChanged(nameof(Background));
            }
        }

        public string Display => IsOn ? "1" : "0";

        public Brush Background => IsOn ? Brushes.LightGreen : Brushes.LightGray;

        public void NotifyUpdated()
        {
            OnPropertyChanged(nameof(Display));
            OnPropertyChanged(nameof(Background));
        }

        public event PropertyChangedEventHandler? PropertyChanged;
        private void OnPropertyChanged(string name) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
    }
}