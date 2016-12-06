const electron = require('electron');
const app = electron.app;
const BrowserWindow = electron.BrowserWindow;
const Menu = electron.Menu;
var ipc = electron.ipcMain;
var count = 0;

let mainWindow




function createWindow() {
  const {width, height} = electron.screen.getPrimaryDisplay().workAreaSize
  mainWindow = new BrowserWindow({width, height});

  mainWindow.loadURL(`file://${__dirname}/index.html`);
  const menuTemplate = [
        {
            label: 'Opções',
            submenu: [
            {
                label: 'Salvar HTML',
                click: () => {
                    console.log('Clicked');
                    //saveHTML();
                    mainWindow.webContents.executeJavaScript(`require('electron').ipcRenderer.send('html', document.getElementById('preview').innerHTML);`);
				ipc.on('html', (_, html) => {
				saveHTML(html)
					})
                }
            }
        ]
        }
    ];
  const menu = Menu.buildFromTemplate(menuTemplate);
  Menu.setApplicationMenu(menu);

  //mainWindow.webContents.openDevTools()
  mainWindow.on('closed', () => {
    mainWindow = null;
  });
}



function saveHTML(html){
	var fs = require('fs');
	// fileName is a string that contains the path and filename created in the save file dialog.  
    fs.writeFile("tests/output/"+ count +".out", html, function (err) {if(err){console.log('erro para salvar')}            
        console.log('salvo com sucesso');
    });
    
}


app.on('ready', createWindow);
