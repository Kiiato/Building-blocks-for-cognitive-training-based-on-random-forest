var app = getApp();
var msg = [394,388,3247,402,417,2801,3204,2941,397,392,387,390,386,390,394,399,51];
var counter = 0;
var CI;
Page({
    /**
     * 页面的初始数据
     */
    data: {
        username:"",
        showmodal:false,
        bggame: "/icon/play.png",
        modebg: {
            "mode1":"https://i.loli.net/2021/04/07/xVscBogD3jhUePF.png",//模型一
            "mode2":"https://i.loli.net/2021/04/07/Z7tXPeEwQlqOokR.png",//模型二
            "mode3":"/icon/mode3.png"//模型三
            //"mode4":"/icon/mode4.png"
        },
        finish_img: {
            "0":"https://i.loli.net/2021/04/06/xPW7G9QAMnuIVLv.png",//差
            "1":"/icon/general.png",//一般
            "2":"/icon/good.png"//优秀
        },
        mode:'',
        forceMatrix:'',
        choosemode: false,
        ischoosed: false,
        BLswitch: false,
        BLdevices:[],
        index: -1,
        finishLevel: 2, //完成等级
        finishState: ['较差，下次一定行','一般，再接再厉','优秀，请继续保持']
    },

    /**
     * 生命周期函数--监听页面加载
     */
    onLoad: function (options) {
        this.setData({
            username: app.globalData.userInfo['ID']
        })
    },

    /**
     * 生命周期函数--监听页面初次渲染完成
     */
    onReady: function () {

    },

    /**
     * 生命周期函数--监听页面显示
     */
    onShow: function () {
        
    },

    /**
     * 生命周期函数--监听页面隐藏
     */
    onHide: function () {

    },

    /**
     * 生命周期函数--监听页面卸载
     */
    onUnload: function () {

    },

    /**
     * 页面相关事件处理函数--监听用户下拉动作
     */
    onPullDownRefresh: function () {

    },

    /**
     * 页面上拉触底事件的处理函数
     */
    onReachBottom: function () {

    },

    /**
     * 用户点击右上角分享
     */
    onShareAppMessage: function () {

    },

    //改变蓝牙状态
    changeBLStatus: function() {
        this.setData({BLswitch:!(this.data.BLswitch)});
        this.openBL();
    },

    //打开蓝牙
    openBL:function() {
        let that = this;
        // 打开蓝牙适配器
        wx.openBluetoothAdapter({
            success(res) {
                console.log(res);
                that.searchBL();
            },
            fail(res) {
                console.log("res")
            }
        });
    },
    //搜索蓝牙
    searchBL: function() {
        let that = this;
        //适配器打开后可以开始搜索蓝牙设备了
        wx.startBluetoothDevicesDiscovery({
            services: [],
            success: function (res) {
                console.log(res);
                //延迟1s后获取搜索到的蓝牙列表
                setTimeout(that.getBL,1000);
            },
            fail: function (res) {
                console.log("搜索蓝牙失败：",res)
            },
        });
    },
    //获取蓝牙
    getBL: function () {
        let that = this;
        //获取搜索到的设备
        wx.getBluetoothDevices({
            success(res) {      
                console.log(res)
                var i=0;
                var devices = [];
                while (res.devices[i]) {
                    devices.push(res.devices[i].name+"  "+res.devices[i].deviceId);
                    i++;
                }
                that.setData({BLdevices:devices});
            },
            fail(res) {
                console.log("搜索失败：",res);
            }
        })
    },

    // 切换picker的选择
    bindPickerChange: function(e) {
        var index = e.detail.value;
        this.setData({index: index});
        if(this.data.BLdevices[index]==undefined) {
            this.setData({index: -1});
        } else {
            console.log("选择的蓝牙成功");
            this.connectBL();
        }  
    },

    //选择对应蓝牙连接
    connectBL: function() {
        var index = this.data.index; //用户选取的蓝牙的index
        var deviceId = this.data.BLdevices[index].split("  ")[1];
        //建立连接
        wx.createBLEConnection({
            deviceId,
            success (res) {
                console.log("连接成功：",res);
            },
            fail(res) {
                console.log("连接失败");
            }
        });
    },

    /**因为使用的是特定的蓝牙所以调试的时候已经获取到了相关id，故建立完连接之后
     * 直接建立监听
     * 相关id如下:
     * serviceId:"0000FFE0-0000-1000-8000-00805F9B34FB"
     * characteristicId:"0000FFE1-0000-1000-8000-00805F9B34FB"
     * */
    //启用低功耗蓝牙设备特征值变化时的 notify 功能，订阅特征值。
    notifyBL:function() {
        var index = this.data.index; //用户选取的蓝牙的index
        var deviceId = this.data.BLdevices[index].split("  ")[1];
        var that = this;
        wx.notifyBLECharacteristicValueChange({
            deviceId:deviceId,
            serviceId:"0000FFE0-0000-1000-8000-00805F9B34FB",
            characteristicId:"0000FFE1-0000-1000-8000-00805F9B34FB",
            state: true,
            success (res) {
                console.log("启用notify功能成功", res);
                //监听蓝牙设备的特征值变化事件
                wx.onBLECharacteristicValueChange((res)=>{
                    console.log("notify监听已经开启");
                    that.transData(res.value);
                });
            },fail(res) {
                console.log("notify",res)
            }
        });
    },
    // 转换数据，将接受到的监听数据转化成字符型
    transData: function(arraybuffer) {
        var that = this;
        var uint8array = new Uint8Array(arraybuffer).slice(0);
        for (var i = 0 ; i < uint8array.length ; i++) {
            msg += String.fromCharCode(uint8array[i]);
        }
        if(msg[msg.length-1]=='q') { 
            console.log(msg);
            that.setData({forceMatrix: msg});
            msg = '';
        }
    },
    
    //点击开始，进行mode选择
    handleTapPlay: function() {
        console.log(1)
        if(this.data.ischoosed==true) {
            //选完了点个锤子
        } else {
            this.setData({choosemode: true});
        }
    },
    //点击图片选择模式处理
    handleTapMode: function(e) {
        var mode = e.currentTarget.id;
        CI = setInterval(this.timing, 1000);
        this.setData({
            bggame: "/icon/"+mode+".png",
            choosemode: false,
            ischoosed: true,
            mode: mode
        });
        this.notifyBL(); //打开监听
    },
    //点击灰色区域
    handleTapGray: function(e) {
        this.setData({
            choosemode: false
        });
    },
    //点击提交按钮
    handleTapSubmit: function() {
        var that = this;
        wx.showModal({
            title: '提示',
            content: '是否提交',
            success (res) {
                wx.offBLECharacteristicValueChange((res)=>{
                    console.log("notify监听已经关闭");//写了一个寂寞
                });
                if (res.confirm) {
                    var msg = that.data.forceMatrix;
                    msg = msg.split(',');
                    console.log('用户提交：');
                    console.log('耗时：',counter);
                    console.log('压力：',that.data.forceMatrix);
                    console.log('模式:', that.data.mode);
                    counter = 0 ;
                    that.setData({
                        bggame: "/icon/play.png",
                        ischoosed: false,
                        forceMatrix: '',
                        //mode: ''
                    });
                    //实验专用
                    msg = [394,388,247,402,417,201,3204,2241,2317,392,387,390,386,390,394,399,50];
                    wx.request({
                        url: 'http://106.14.150.138/jm.php', //调用模型接口
                        data: {
                            0:msg[0],
                            1:msg[1],
                            2:msg[2],
                            3:msg[3],
                            4:msg[4],
                            5:msg[5],
                            6:msg[6],
                            7:msg[7],
                            8:msg[8],
                            9:msg[9],
                            10:msg[10],
                            11:msg[11],
                            12:msg[12],
                            13:msg[13],
                            14:msg[14],
                            15:msg[15],
                            16:msg[16],
                            mode: that.data.mode,
                            username: that.data.username
                        },
                        
                        header: {
                            'content-type': 'application/x-www-form-urlencoded' // 更改为query string
                        },
                        method:"POST",
                        success (res) {
                            console.log("接收到的数据为：",res);
                            that.setData({
                                finishLevel: (Number(res.data['result'])-1),
                                showmodal: true,
                                mode: ''
                            });
                        },
                        fail (res){
                            console.log("接收失败",res)
                        }
                    })
                  
                    clearInterval(CI);
                }else if (res.cancel) {
                    console.log('用户点击取消')
                }
            }
        })  
    },
    //点击结束按钮
    handleTapEnd: function() {
        var that = this;
        wx.showModal({
            title: '提示',
            content: '是否结束',
            success (res) {
                if (res.confirm) {
                    wx.offBLECharacteristicValueChange((res)=>{
                        console.log("notify监听已经关闭");//写了一个寂寞
                    });
                    that.setData({
                        bggame: "/icon/play.png",
                        ischoosed: false
                    });
                } else if (res.cancel) {
                    console.log('用户点击取消')
                }
            }
        })
    },
    //计时
    timing: function() {
        counter += 1;
    },
    //关闭弹窗
	closeModal: function() {
        this.setData({showmodal: false});
	}
})