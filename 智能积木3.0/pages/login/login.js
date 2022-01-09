var app = getApp();
Page({
    /**
     * 页面的初始数据
     */
    data: {
        uname: "",
        signin_name: "",
        signin_pwd: "",
        signin_pwd_asure: "",
        pwd: "",
        signIn:{    //注册相关组件
            isShow: "",
        }
    },

    /**
     * 生命周期函数--监听页面加载
     */
    onLoad: function (options) {
        // //当页面加载时，获取缓存里的学号密码
        // var id = wx.getStorageSync('uname');
        // var pwd = wx.getStorageSync('pwd');
        // wx.request({
        //     url: "http://106.14.150.138/userlogin.php",
        //     header: { 'content-type': 'application/x-www-form-urlencoded' },
        //     data: {
        //         "uname": id,
        //         "pwd": pwd
        //     },
        //     method: 'POST',
        //     success(res) {
        //         var d = res.data;
        //         console.log(d);
        //         if (d == -1) {
        //             console.log("本地信息过期或者不存在");
        //         } else {
        //             //登录成功时，获取数据
        //             console.log(res)
        //             app.globalData.userInfo = res.data;
        //             //进入主界面
        //             wx.switchTab({
        //                 url: '/pages/game/game',
        //             })
        //         }
        //     }
        // });
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

    //获取输入的用户名
    getUserName(e) {
        var value = e.detail.value; //获取输入的内容
        this.setData({
            uname: value,
        });
        wx.setStorageSync('uname', value);//将获取到的username值存入本地缓存空间
    },

    //获取输入的密码
    getPassword(e) {
        var value = e.detail.value;
        this.setData({
            pwd: value
        });
        wx.setStorageSync('pwd', value);
    },

    //获取注册输入的用户名
    getUserName_signin(e) {
        var value = e.detail.value; //获取输入的内容
        this.setData({
            signin_name: value,
        });
    },

    //获取注册输入的密码
    getPassword_signin(e) {
        var value = e.detail.value;
        this.setData({
            signin_pwd: value
        });
    },
    //获取注册输入的确认密码
    getPassword_asure_signin(e) {
        var value = e.detail.value;
        this.setData({
            signin_pwd_asure: value
        });
    },

    //登录
    dologin(e) {
        let that = this;
        if (this.data.uname.length == 0 || this.data.pwd.length == 0) {	//如果没有输入
            wx.showToast({
                title: "请输入用户名或密码",
                icon: "none"
            });
        } else {
            that.setData({ loadStatus: true });
            //发送登陆请求
            wx.request({
                url: "http://106.14.150.138/userlogin.php",
                header: { 'content-type': 'application/x-www-form-urlencoded' },
                data: {
                    "uname": that.data.uname,
                    "pwd": that.data.pwd
                },
                method: 'POST',
                success(res) {
                    var d = res.data;
                    that.setData({ loadStatus: false });
                    if (d == -1) {	//返回-1说明用户名密码错误
                        wx.showToast({
                            title: "用户名或密码输入错误",
                            icon: "none"
                        });
                    } else {
                        //登录成功时，获取数据
                        console.log(res)
                        app.globalData.userInfo = res.data;
                        //进入主界面
                        wx.switchTab({
                            url: '/pages/game/game',
                        })
                    }
                }
            });
        }
    },

    //注册
    dosignin(e) {
        let that = this;
        if (this.data.signin_name.length == 0 || this.data.signin_pwd.length == 0 || this.data.signin_pwd_asure.length == 0) {	//如果没有输入
            wx.showToast({
                title: "请输入合法用户名和密码",
                icon: "none"
            });
            
        }else if(!(this.data.signin_pwd===this.data.signin_pwd_asure)){
            wx.showToast({
                title: "请重新输入密码",
                icon: "none"
            });
        }else {
            that.setData({ loadStatus: true });
            //发送登陆请求
            wx.request({
                url: "http://106.14.150.138/register.php",
                header: { 'content-type': 'application/x-www-form-urlencoded' },
                data: {
                    "uname": that.data.signin_name,
                    "pwd": that.data.signin_pwd
                },
                method: 'POST',
                success(res) {
                    var d = res.data;
                    that.setData({ loadStatus: false });
                    // if (d == -1) {	//返回-1说明用户名密码错误
                    //     wx.showToast({
                    //         title: "用户名或密码输入错误",
                    //         icon: "none"
                    //     });
                    // } else {
                        //注册成功时，获取数据
                        console.log(res);
                        app.globalData.userInfo = res.data;
                        //注册成功时的提示信息
                        wx.showToast({
                            title: "注册成功！",
                            icon: "none"
                        });
                        setTimeout(function(){    
                            //进入注册页面
                            wx.reLaunch({
                                url: '/pages/login/login',
                            })
                        },2000);
                        
                    //}
                }
            });
        }
    },
    handleTapSignin: function() {
        this.setData({signIn: {isShow:"show"}});
    },
    handleTapX: function() {
        this.setData({signIn: {isShow:""}});
    }
})