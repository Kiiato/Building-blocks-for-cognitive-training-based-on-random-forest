import * as echarts from '../../ec-canvas/echarts';
var app = getApp();
Page({
    /**
     * 页面的初始数据
     */
    data: {
        username:"",
        flat: "",
        index: -1,
        pickerArray: ['近七天每日平均得分','近七天总平均得分'],
        ec: {
			lazyLoad: true // 延迟加载
        },
        text: `<div style='color: green;'>很牛</div>
            <div  style='color: red;'>不行</div>`,
        mode1_data:[],
        mode2_data:[],
        mode3_data:[],
        mode1_data_aver:0,
        mode2_data_aver:0,
        mode3_data_aver:0,
        date_data:[]
    },

    /**
     * 生命周期函数--监听页面加载
     */
    onLoad: function (options) {
        var that = this;
        that.setData({
            username:app.globalData.userInfo['ID']
        })
        that.echartsComponent = that.selectComponent('#mychart-dom');
        that.get_history_data();
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
   	//切换picker的项
	bindPickerChange: function(e) {
        var that = this;
		that.setData({
		  index: e.detail.value
        });
        that.initChart();
    },
    onPullDownRefresh: function() {
        var that = this;
        wx.stopPullDownRefresh();
        that.initChart();
        //wx.showNavigationBarLoading() //在标题栏中显示加载
    },
    //初始化折线图
	initChart: function () {
        var that = this;
		var index = that.data.index;
		that.echartsComponent.init((canvas, width, height) => {
            that.get_history_data();
            //while(that.mode1_data==null){};
            // 初始化图表
            const Chart = echarts.init(canvas, null, {
                width: width,
                height: height
            });
		
            var option = [];
			option[0] = {
                tooltip: {
                    trigger: 'axis'
                },
                legend: {
                    type:'plain',
                    show: 'true',
                    data: ['模型1','模型2','模型3','模型4']
                },
				xAxis: {
					type: 'category',
					axisTick: {
						alignWithLabel:true
					},
					data: that.data.date_data
				},
				yAxis: {
					name: '平均得分',
					type: 'value',
				},
				series: [
                    {
                        name: '模型1',
					    data: that.data.mode1_data,
					    type: 'line'
                    },
                    {
                        name:"模型2",
					    data: [9,7,6.3,8.9,9,9.9,9],
					    type: 'line'
                    },
                    {
                        name:"模型3",
					    data: [10,10,10,10,10,10,10],
					    type: 'line'
                    },
                    // {
                    //     name:"模型4",
					//     data: [6,8,9,9,7.5,8,10],
					//     type: 'line'
                    // }
                ]
            };
            option[1] = {
                tooltip: {
                    trigger: 'axis'
                },
                legend: {
                    type:'plain',
                    show: 'true',
                    //data: ['正确率','错误率']
                    data: ['总平均得分']
                },
				xAxis: {
                    name: '模型',
					type: 'category',
					data: ['模型1','模型2','模型3']
				},
				yAxis: {
					name: '总平均得分',
					type: 'value',
				},
				series: [
                    {
                        name: '总平均得分',
                        stack: '总计',
                        data: [that.data.mode1_data_aver,7,6],
                        itemStyle: {
                            color: 'rgb(145,204,117)'
                        },
					    type: 'bar'
                    },
                    // {
                    //     name:"错误率",
                    //     stack: '总计',
                    //     data: [20,30,40,12],
                    //     itemStyle: {
                    //         color: 'rgb(244,105,105)'
                    //     },
					//     type: 'bar'
                    // }
                ]
            };
            Chart.setOption(option[index]);
			return Chart;
		});
    },
    //获取历史数据
    get_history_data:function() {
        var that = this;
        var tmp_data=[];
        var tmp_data_aver=0;
        wx.request({
            url: 'http://106.14.150.138/history.php', //历史数据请求接口
            data: {
                username: that.data.username
            },
            header: {
                'content-type': 'application/x-www-form-urlencoded' // 更改为query string
            },
            method:"POST",
            success (res) {
                console.log("接收到的数据为：",res.data);
                tmp_data_aver=0;
                for(var i=0;i<7;i++){
                    tmp_data[i] = Number(res.data['score_mode1'][i]);
                    tmp_data_aver=tmp_data_aver+tmp_data[i];
                };

                tmp_data_aver=(tmp_data_aver/7).toFixed(2);

                that.setData({
                    mode1_data: tmp_data,
                    date_data: res.data['date_mode1'],
                    mode1_data_aver: tmp_data_aver
                  });
            },
            fail (res){
                console.log("接收失败",res)
            }
        })
    }

})