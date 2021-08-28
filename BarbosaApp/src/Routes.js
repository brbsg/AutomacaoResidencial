import {createAppContainer, createSwitchNavigator} from 'react-navigation'

import LoggedScreen from './Pages/LoggedScreen'
import LoginPage from './Pages/LoginPage'

const Routes = createAppContainer(
    createSwitchNavigator(
        {LoginPage, LoggedScreen},
        {initialRouteName: 'LoggedScreen', backBehavior: 'history'}    
    ),
)

export default Routes